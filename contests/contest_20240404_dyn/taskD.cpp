#include <iostream>

using std::cout;
using std::cin;


int main()
{
    long N;
    cin >> N;
    long array[100];
    array[0] = 1;
    array[1] = 1;
    array[2] = 2;
    for(long i = 3; i<=N; i++)
    {
        array[i] = array[i-1] + array[i-2] + array[i-3];
    }
    cout << array[N];
}