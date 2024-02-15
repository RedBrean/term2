#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

char* check(long long x)
{   
    int maxi = floor(sqrt(x))+5;
    for(int i = 2; i < maxi; i++)
    {
        if(x % i == 0 && i<x){return "NO";}
    }
    return "YES";
}

int main()
{
    long long x;
    cin >> x;
    cout << check(x);
}
