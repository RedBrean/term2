#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int fac(long long x)
{
    if(x == 0) {return 1;}
    return fac(x-1)*x;
}

int main()
{
    long long x;
    cin >> x;
    cout << fac(x);
}
