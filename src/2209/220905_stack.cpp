#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main()
{
    string str;
    getline(cin, str);

    stack<char> st;

    for(auto &i : str)
    {
        if(i == '(')
        {
            st.push(i);
        }
        else if(i == ')')
        {
            if(!st.empty())
            {   
                st.pop();
            }
            else
            {
                cout << "括号不匹配" << endl;
                return 0;
            }
            
        }
    }

    if(st.empty())
    {   
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
    
    return 0;

}