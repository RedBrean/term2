#include <iostream>

using namespace std;

void solve(int a, int b, int c, int& x, int& y)
{
    bool sw = false;
    if(a>b){
        auto temp = a;
        a = b;
        b = temp;
        sw = true;
    }

    x = 0; y = 0;
    for (int i = 0; i<abs(b); i++){
        if((i*a - c) % b == 0)
        {
            x = i;
            y = -(i*a-c)/b;
        }
    }   
    if(sw){        
        auto temp = x;
        x = y;
        y = temp;
    }
}

