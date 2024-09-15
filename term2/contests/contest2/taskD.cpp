#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int pow(int x, int y)
{
    if(x == 0) {return 0;}
    if(y == 0) {return 1;}
    return pow(x, y-1) * x;
}

int main()
{
    int N, M;
    cin >> N >> M;
    cout << pow(N, M);
}
