#include <iostream>
#include <math.h>

using namespace std;

int fib(long long x)
{
    if(x == 0 | x == 1) {return 1;}
    return fib(x-1) + fib(x-2);
}

int main()
{
    long long x;
    cin >> x;
    cout << fib(x);
}
