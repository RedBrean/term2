#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;



int main()
{
    long long M,N;
    cin >> M >> N;
    long long a = 2;
    while(true)
    {
        a = N/M;
        if(N == M*a){
            cout << a;
            break;
        }
        else{a++;}
        cout << a << " ";
        M = M*a - N;
        N *= a;

    }

}
