#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <sstream>
#include <regex>
#include <windows.h>
using namespace std;
stack<char> opers;
stack<long double> nums;
char op[]{'|', '^', '&', '+', '-', '%', '/', '*', '('};
int prioritys[]{1, 2, 3, 4, 4, 5, 5, 5, 0};
regex reNum("-?\\d+\\.?\\d*");
regex reOper("[-+*/%|&^()]");
map<char, int> dic;
inline bool isOperator(char);
inline bool isNumOrDot(char);
void run();
long double calc(char, const long double &, const long double &);
void printHelp();
bool checkStr(const string &);
void InitPriority();
void ProcessStr(const string &);
void ProcessExp(char);
void CopyResult();
string last_result="";
class RAIIofClipboard{
public:
    RAIIofClipboard(){
        OpenClipboard(NULL);
        EmptyClipboard();
    }
    ~RAIIofClipboard(){
        CloseClipboard();
    }
};
int main()
{
    string in;
    InitPriority();
    printHelp();
    while (1)
    {
        cout << ">>>";
        cin >> in;
        if (in == "\\q" || in == "quit")
        {
            return 0;
        }
        else if (in == "help" || in == "\\h")
        {
            printHelp();
            continue;
        }else if(in=="\\c"){
            CopyResult();
            continue;
        }
        if (checkStr(in))
        {
            ProcessStr(in);
        }
        else
        {
            cout << "Illegal expression!" << endl;
            continue;
        }
        while (!opers.empty())
            run();
        last_result=to_string(nums.top());
        cout << last_result << endl;
        nums.pop();
    }
    return 0;
}

void InitPriority(){
    for (int i = 0; i < 9; i++)
    {
        dic[op[i]] = prioritys[i];
    }
}

bool isOperator(char a)
{
    return a == '|' || a == '&' || a == '^' || a == '+' || a == '-' || a == '*' || a == '/' || a == '%' || a == '(' || a == ')';
}

bool isNumOrDot(char a)
{
    return (a <= 57 && a >= 48) || a == '.';
}

bool checkStr(const string &str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if ((!isOperator(str[i])) && (!isNumOrDot(str[i])))
        {
            return false;
        }
    }
    return true;
}

void printHelp()
{
    cout << "Type \"quit\" or \'\\q\' to quit." << endl;
    cout << "Type mathmatix expression to calculate." << endl;
    cout << "Type \"help\" or '\\h' to show help." << endl;
    cout << "Available math symbols:{'|', '^', '&', '+', '-', '%', '/', '*'}." << endl;
    cout << "Type \'\\c\' to copy the result to Clipboard."<<endl;
}

void CopyResult(){
    RAIIofClipboard raii;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, last_result.length()+1);
    LPTSTR lpMem=(LPTSTR)GlobalLock(hMem);
    memcpy(lpMem, last_result.data(), last_result.length());
    GlobalUnlock(hMem);
    SetClipboardData(CF_TEXT, hMem);
    cout << "Copied the result." << endl;
}

void run()
{
    long double &tmp = nums.top();
    nums.pop();
    long double &&res = calc(opers.top(), nums.top(), tmp);
    nums.pop();
    opers.pop();
    nums.push(res);
}

long double calc(char rel, const long double &lhs, const long double &rhs)
{
    switch (rel)
    {
    case '|':
        return (long long)lhs | (long long)rhs;
    case '^':
        return (long long)lhs ^ (long long)rhs;
    case '&':
        return (long long)lhs & (long long)rhs;
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    case '*':
        return lhs * rhs;
    case '/':
        return lhs / rhs;
    case '%':
        return (long long)lhs % (long long)rhs;
    }
    return 0;
}

void ProcessExp(char c){
    switch (c)
    {
        case ')':
            while (opers.top() != '(')
                run();
            opers.pop();
            break;
        case '(':
            opers.push(c);
            break;
        default:
            while (!opers.empty() && dic[opers.top()] >= dic[c])
                run();
            opers.push(c);
            break;
    }
}

void ProcessStr(const string &str){
    istringstream iss;
    smatch m1, m2;
    string::const_iterator beg = str.begin();
    string::const_iterator rear = str.end();
    long double number;
    while (regex_search(beg, rear, m1, reNum))
    {
        iss.str(m1[0]);
        iss >> number;
        iss.clear();
        nums.push(number);
        beg = m1[0].second;
        while (regex_search(beg, rear, m2, reOper))
        {
            ProcessExp(m2[0].str()[0]);
            beg = m2[0].second;
            if (!(m2.suffix().length() && isOperator(m2[0].second[0])))
            {
                break;
            }
        }
    }
}