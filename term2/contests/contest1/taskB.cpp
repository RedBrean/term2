#include <iostream>
#include <math.h>

int main()
{
    int a, b, c;

    std::cin >> a >> b >> c;
    if(a == 0)
    {
        if (b == 0) {return 0;}
        std::cout << -(double)c/(double)b;
        return 0;
    }

    double D = b*b - 4*a*c;

    if (D == 0)
    {
        std::cout << -b / 2 /a;
    }
    else if (D < 0)
    {
        
    }
    else if (D > 0)
    {
        double x1 = (-b - std::sqrt(D))/2/a;
        double x2 = (-b + std::sqrt(D))/2/a;
        std::cout << x1 << " " << x2;
    }
}

