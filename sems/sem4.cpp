#include <iostream>

//define полезный

struct loong{
    int* digits;
    int size;
};

using std::cout;
using std::cin;

loong plus(loong a, loong b)
{
    loong out;
    int max_size = a.size > b.size ? a.size : b.size;
    out.digits = new int[max_size+1];
    bool perenos = 0;
    for(int i = 0; i<=max_size; i++)
    {
        int digit = perenos + a.digits[i] + b.digits[i];
        if (digit >= 10){
            perenos = 1;
            digit -= 10;
            }
        else {perenos = 0;}
        out.digits[i] = digit;
    }
    out.size = out.digits[max_size] == 0 ? max_size : max_size + 1;
    return out;
}

void print_long(loong a)
{
    for(int i = a.size-1; i >= 0; i--)
    {
        cout<<a.digits[i];
    }
}

loong read_long(int size)
{
    loong result;
    result.size = size;
    result.digits = new int[size];
    char symbol;
    for (int i = size - 1; i>=0; i--)
    {
        cin >> symbol;
        result.digits[i] = (int)symbol - (int)'0';
    }
    return result;
}

int main()
{
    cout << "Сколько знаков в числе 1: ";
    int size_a;
    cin >> size_a;
    loong a = read_long(size_a);
    cout << "Сколько знаков в числе 2: ";
    int size_b;
    cin >> size_b;
    loong b = read_long(size_b);
    print_long(plus(a, b));
    return 0;

}