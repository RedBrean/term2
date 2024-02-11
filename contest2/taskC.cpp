#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

bool check(int x)
{
    if(x == 1){ return 1; }

    if(x<=0 | x % 2 == 1)    { return 0;} 
    else    { return check(x/2); }
}

int main()
{
    int x;
    cin >> x;

    auto output = check(x) ? "YES" : "NO";

    std::cout << output;

}
