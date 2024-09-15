#include <iostream>
#include <math.h>
using std::cout;
using std::cin;

long long F(int n)
{
    n++;
    double a = std::pow(((1+std::sqrt(5))/2), n);
    double b = std::pow(((1-std::sqrt(5))/2), n);
    return (std::round((a-b)/std::sqrt(5)));
}

int main()
{
    int N;
    cin >> N;
    auto f = F(N);
    cout << f*f;
}