#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>
typedef long long ll;

using std::cout;
using std::cin;
using std::vector;
using std::string;

bool pattern_match(char const *pattern, char const *str)
{
    int i = 0;
    int j = 0;
    while (true)
    {   
        if(pattern[i] == '0'){return(str[j] == '0');}
        if(str[j] == '0'){return(pattern[i] == '0');}
        char symb = pattern[i];
        if(symb == '+'){
            i++;
            continue;
        }
        bool inf = pattern[i+1] == '+';
        if(inf){
            while(str[j] == symb){
                j++;
                if(str[j] == '0'){
                    break;
                }
            }
        }
        else{
            if(str[j]==symb){j++;}
            else{return 0; }
        }
        i++;
    }
    
}

int main()
{
    cout << pattern_match("a+b+0","aaaabbbbb0");
}