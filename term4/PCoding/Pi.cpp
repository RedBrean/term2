#include <math.h>
#include <iostream>

long long N;

int main()
{
    std::cout << "Enter N: ";
    std::cin >> N;
    std::cout << std::endl;

    double dx = 2.0/N;
    double Pi = 0;

    for(double x = 0; x<=2; x+=dx)
    {
        Pi+=dx*sqrt(4-x*x);
    }

    std::cout << "Pi = " << Pi << std::endl;

    return 0;
}