#include <iostream>
#include <math.h>

int main()
{
    int year;

    std::cin >> year;

    bool vesokos = (year % 4 == 0) & ((year % 100 != 0) | (year % 400 == 0));

    auto output = vesokos ? "YES" : "NO";

    std::cout << output;
    return 0;
}

