#include <iostream>
#include <fstream>
#include<sstream>
#include <math.h>
#include <iomanip>
#include <stack>
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
//  int Rank(c) trả về độ ưu tiên của toán tử, toán hạng c => chỉ dùng để phục vụ hàm CheckExpression(); không cần quan tâm
//  bool CheckExpression(string s) trả về true nếu string s là phương trình hợp lí, false nếu không
// string postfix(string s) trả về chuỗi là hậu tố từ phương trình trung tố s
// float calculation(string s) trả về giá trị float cho việc tính toán, đầu vào string s phải là hậu tố có từ hàm postfix phía trên nên khi code gọi hàm này thì nhớ phải cho string gán bằng hàm postfix phía trên trước
}
