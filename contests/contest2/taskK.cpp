#include <iostream>
#include <vector>
using namespace std;



long long NOD(long long a, long long b)
{
    while(a>0)
    {
        if(b>a)
        {
            auto tmp = a;
            a = b;
            b = tmp;
        }
        a = a - b;
    }
    return b;

}

long long phi(long long x)
{
    long long c = 0;
    for (int i = 1; i<x; i++)
    {
        if(NOD(x, i) == 1){c++;}

    }
    return c;

}

long long pow(long long a, long long k, long long m)
{
    long long res = 1;
    for (int i = 0; i<k; i++)
    {
        res = (res*a)%m;
    }
    return res;
}
long long superpow(long long a, long long k, long long m)
{   
    long long m_phi = phi(m);
    long long res = a;
    for (long long i = 0; i<k-1; i++)
    {
        res = pow(a, res, m)%m_phi;
    }
    return res%m;

}
int main()
{
    long long a, k, m;
    cin >> a >> k >> m;

    cout << superpow(a, k, m);


}