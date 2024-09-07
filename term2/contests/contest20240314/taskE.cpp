#include <iostream>
using std::cin;
using std::cout;
struct Complex {
    int re;
    int im;
};

Complex complexMul(const Complex& lhs, const Complex& rhs)
{
    return Complex{.re = lhs.re * rhs.re - lhs.im * rhs.im, .im = lhs.re * rhs.im + lhs.im * rhs.re};
}
int main()
{
    Complex a, b;
    cin >> a.re >> a.im >> b.re >> b.im;
    auto c = complexMul(a, b);
    cout << c.re << " " << c.im;

}