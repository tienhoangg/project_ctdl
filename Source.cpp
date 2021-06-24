#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
using namespace std;

template <class T>
struct node
{
    T value;
    node *next;
};

template <class T>
class stack
{
private:
    node<T> *p_high = NULL;
    int count = 0;

public:
    ~stack()
    {
        while (count > 0)
            this->pop();
    }
    void push(T);
    void pop();
    T top();
    int size();
};

template <class T>
void stack<T>::push(T value)
{
    node<T> *temp;
    temp = new node<T>;
    temp->value = value;
    temp->next = p_high;
    p_high = temp;
    count++;
}

template <class T>
void stack<T>::pop()
{
    if (p_high == NULL)
        return;
    node<T> *temp;
    temp = p_high;
    p_high = p_high->next;
    delete temp;
    count--;
}

template <class T>
T stack<T>::top()
{
    if (p_high != NULL)
        return p_high->value;
}

template <class T>
int stack<T>::size()
{
    return count;
}

int Rank(char c)
{
    if (c == ' ')
        return 0;
    if (c >= '0' && c <= '9')
        return 1;
    if (c == '+' || c == '-')
        return 2;
    if (c == '*' || c == '/' || c == '^')
        return 3;
    if (c == '(')
        return 4;
    if (c == ')')
        return 5;
    if (c == '.')
        return 6;
}

bool CheckExpression(string s)
{
    if (Rank(s[0]) == 2 || Rank(s[0]) == 3 || Rank(s[0]) == 6)
        return false;
    int check = 0;
    bool dot = true;
    stack<char> temp;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{')
        {
            temp.push(s[i]);
            s[i] = '(';
            dot = true;
        }
        if (s[i] == ')' || s[i] == ']' || s[i] == '}')
        {
            if (temp.size() < 1)
                return false;
            if (s[i] == ')' && temp.top() != '(')
                return false;
            if (s[i] == ']' && temp.top() != '[')
                return false;
            if (s[i] == '}' && temp.top() != '{')
                return false;
            temp.pop();
            s[i] = ')';
            dot = true;
        }
        if (check == 1)
        {
            if (Rank(s[i]) == 1 && Rank(s[i - 1]) == 0)
                return false;
            if (Rank(s[i]) == 4)
                return false;
        }
        if (check == 2 || check == 3)
        {
            if (Rank(s[i]) == 2 || Rank(s[i]) == 3)
                return false;
            dot = true;
        }
        if (check == 4)
        {
            if (Rank(s[i]) == 2 || Rank(s[i]) == 3 || Rank(s[i]) == 5)
                return false;
            dot = true;
        }
        if (check == 5)
        {
            if (Rank(s[i]) == 1 || Rank(s[i]) == 4)
                return false;
            dot = true;
        }
        if (check == 6)
        {
            if (Rank(s[i]) != 1)
                return false;
            if (!dot)
                return false;
            dot = false;
        }
        if (Rank(s[i]) > 0)
            check = Rank(s[i]);
    }
    if (temp.size() != 0)
        return false;
    return true;
}

string postfix(string s)
{
    string s_new = "";
    stack<char> temp;
    for (int i = 0; i < s.length(); i++)
    {
        if (Rank(s[i]) == 0 && s_new[s_new.length() - 1] != ' ')
        {
            s_new += s[i];
            continue;
        }
        if (Rank(s[i]) == 1 || Rank(s[i]) == 6)
        {
            s_new += s[i];
            continue;
        }
        if (Rank(s[i]) == 2)
        {
            if (temp.size() > 0 && Rank(temp.top()) <= 3 && Rank(temp.top()) >= 2)
            {
                if (s_new[s_new.length() - 1] != ' ')
                    s_new += ' ';
                s_new += temp.top();
                temp.pop();
            }
            temp.push(s[i]);
            continue;
        }
        if (Rank(s[i]) == 3)
        {
            if (temp.size() > 0 && Rank(temp.top()) == 3)
            {
                if (s_new[s_new.length() - 1] != ' ')
                    s_new += ' ';
                s_new += temp.top();
                temp.pop();
            }
            temp.push(s[i]);
            continue;
        }
        if (Rank(s[i]) == 4)
            temp.push(s[i]);
        if (Rank(s[i]) == 5)
        {
            while (temp.top() != '(')
            {
                if (s_new[s_new.length() - 1] != ' ')
                    s_new += ' ';
                s_new += temp.top();
                temp.pop();
            }
            temp.pop();
        }
    }
    while (temp.size() > 0)
    {
        if (s_new[s_new.length() - 1] != ' ')
            s_new += ' ';
        s_new += temp.top();
        temp.pop();
    }
    return s_new;
}

bool calculation(string s, float &output)
{
    stack<float> temp;
    stringstream ss(s);
    float f, float_temp1, float_temp2;
    char c;
    while (!ss.eof())
    {
        ss >> c;
        if (c >= '0' && c <= '9')
        {
            ss.seekg(-1, ios::cur);
            ss >> f;
            temp.push(f);
            continue;
        }
        if (temp.size() >= 2)
        {
            float_temp2 = temp.top();
            temp.pop();
            float_temp1 = temp.top();
            temp.pop();
            if (c == '+')
                temp.push(float_temp1 + float_temp2);
            if (c == '-')
                temp.push(float_temp1 - float_temp2);
            if (c == '*')
                temp.push(float_temp1 * float_temp2);
            if (c == '/')
                if (float_temp2 != 0)
                    temp.push(float_temp1 / float_temp2);
                else
                    return false;
            if (c == '^')
                temp.push(pow(float_temp1, float_temp2));
        }
    }
    output = temp.top();
    return true;
}

int main(int argc, char *argv[])
{
    ifstream input;
    ofstream output;
    string PostFix;
    input.open(argv[1]);
    string a;
    float cal;
    if (string(argv[3]) == "-c")
    {
        output.open(argv[4]);
        for (int i = 0; i < stoi(argv[2]); i++)
        {
            getline(input, a);
            if (CheckExpression(a) == true)
            {
                PostFix = postfix(a);
                if (calculation(PostFix, cal) == true)
                {
                    output << setprecision(3) << cal << endl;
                }
                else
                    output << "E" << endl;
            }
            else
            {
                output << "E" << endl;
            }
        }
    }
    else if (string(argv[3]) == "-t")
    {
        output.open(argv[4]);
        for (int i = 0; i < stoi(argv[2]); i++)
        {

            getline(input, a);
            if (CheckExpression(a) == true)
            {
                PostFix = postfix(a);
                output << PostFix << endl;
            }
            else
            {
                output << "E" << endl;
            }
        }
    }
    input.close();
    output.close();
}