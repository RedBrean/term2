#include <iostream>
using std::cin;
using std::cout;
struct vector
{
    int* coords;
    int n;
    vector(int N)
    {
        coords = new int[N];
        n = N;
    }
    void print()
    {
        for(int i = 0; i < n; i++)
        {
            cout << coords[i] << " ";
        }
    }
    vector operator + (vector another)
    {
        vector result(n);
        for (int i = 0; i < n; i++)
        {
            result.at(i) = coords[i] + another.at(i);
        }
        return result;
    }
    int& at(int i)
    { return coords[i]; }
};



int main()
{
    int N;
    cin >> N;
    vector first(N);
    vector second(N);
    for (int i = 0; i < N; i++)
    {
        cin >> first.at(i);
    }
    for (int i = 0; i < N; i++)
    {
        cin >> second.at(i);
    }
    cout << first.n << " ";
    (first + second).print();
}