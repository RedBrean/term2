#include <iostream>

int main()
{
    short  a=5, b=5;
    auto c = a+b;
    std::cout << sizeof(c) << std::endl;
    std::cout << sizeof(static_cast<short>(c)) << "\n";

    int d;
    std::cout << d;
}