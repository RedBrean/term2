#include <iostream>
int NOD(int a, int b)
{
    if (a <= 0)
    { return b; }
    else if (b <= 0)
    { return a; }
    else if(a > b)
    { return NOD(a-b, b); }
    else
    { return NOD(a, b-a); }
}

int main()
{
    int a, b;
    std::cin >> a >> b;
    std::cout << NOD(a,b);

}