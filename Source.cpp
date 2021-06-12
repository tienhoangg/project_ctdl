#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <stack>
#include <iomanip>
using namespace std;

int Rank(char c)
{
    if (c == ' ')
        return 0;
    if ((c >= '0' && c <= '9') || c == '.')
        return 1;
    if (c == '+' || c == '-')
        return 2;
    if (c == '*' || c == '/' || c == '^')
        return 3;
    if (c == '(')
        return 4;
    if (c == ')')
        return 5;
}

bool CheckExpression(string s)
{
    int check = 0, bracket1 = 0, bracket2 = 0, bracket3 = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
            bracket1++;
        if (s[i] == ')')
            bracket1--;
        if (s[i] == '[')
        {
            bracket2++;
            s[i] = '(';
        }
        if (s[i] == ']')
        {
            bracket2--;
            s[i] = ')';
        }
        if (s[i] == '{')
        {
            bracket3++;
            s[i] = '(';
        }
        if (s[i] == '}')
        {
            bracket3--;
            s[i] = ')';
        }
        if (bracket1 < 0 || bracket2 < 0 || bracket3 < 0)
            return false;
        if (check == 1)
        {
            if (Rank(s[i]) == 1 && Rank(s[i - 1]) == 0)
                return false;
        }
        if (check == 2 || check == 3)
        {
            if (Rank(s[i]) == 2 || Rank(s[i]) == 3)
                return false;
        }
        if (check == 4)
        {
            if (Rank(s[i]) == 2 || Rank(s[i]) == 3)
                return false;
        }
        if (check == 5)
        {
            if (Rank(s[i]) == 1)
                return false;
        }
        if (Rank(s[i]) > 0)
            check = Rank(s[i]);
    }
    if (bracket1 != 0 || bracket2 != 0 || bracket3 != 0)
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
        if (Rank(s[i]) == 1)
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

float calculation(string s)
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
                temp.push(float_temp1 / float_temp2);
            if (c == '^')
                temp.push(pow(float_temp1, float_temp2));
        }
    }
    return temp.top();
}

int main()
{
    ifstream inPut;
    ofstream outPut;
    string s, s1, PostFix;
    string choice;
    int n;
    cout << "nhap file txt input: " << endl;
    cin >> s;
    inPut.open(s);
    cout << "nhap file txt output: " << endl;
    cin >> s1;
    outPut.open(s1);
    cout << "nhap so luong phep tinh: " << endl;
    cin >> n;
    cout << "chon hanh dong: " << endl;
    cout << "c: tinh toan " << endl;
    cout << "t: chuyen doi" << endl;
    cin >> choice;
    string *a = new string[n];
    float *cal = new float[n];
    if (choice == "c")
    {
        for (int i = 0; i < n; i++)
        {

            getline(inPut, a[i]);
            if (CheckExpression(a[i]) == true)
            {
                PostFix = postfix(a[i]);
                cal[i] = calculation(PostFix);
                outPut << setprecision(3) << cal[i] << endl;
            }
            else
            {
                outPut << "E" << endl;
            }
        }
    }
    else if (choice == "t")
        for (int i = 0; i < n; i++)
        {
            getline(inPut, a[i]);
            if (CheckExpression(a[i]) == true)
            {
                PostFix = postfix(a[i]);
                outPut << PostFix << endl;
            }
            else
            {
                outPut << "E" << endl;
            }
        }
    else
        cout << "nhap khong hop le " << endl;
}