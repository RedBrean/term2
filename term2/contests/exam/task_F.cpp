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

ll count(string mask, ll N)
{
    if(mask == "000")
    {
        if(N%2 == 1){
            return 0;
        }
        if(N > 10){
            ll k = N/2;
            vector<string> shvi{"000", "001", "010", "011", "100", "101", "110", "111"};
            ll sum = 0;
            for (int i = 0; i < 8; i++)
            {
                ll c = count(shvi[i], k);
                sum += c*c;
            }
            return sum;
        }
    }
    if(N==0)
    {
        if(mask == "000"){return 1;}
        else{return 0;}
    }
    if(N == 1)
    {    
        if(mask == "100" || mask == "001" || mask == "111")
        {return 1;}
        else{ return 0; }
    }
    else{
        if(mask == "111"){return count("000", N-1);}
        if(mask == "110" || mask == "011"){return count("001", N-1);}
        if(mask == "101"){return count("010", N-1);}
        if(mask == "100" || mask == "001"){return count("000", N-1) + count("011", N-1);}
        if(mask == "000") {return count("111", N-1) + count("100", N-1) + count("001", N-1);}
    }
    return 0;
}


int main()
{
    ll N;
    cin >> N;
    cout << count("000", N);
}