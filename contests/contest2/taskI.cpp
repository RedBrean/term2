#include <iostream>
#include <vector>
using namespace std;

int solve(int x)
{
    int i = 0;
    int p = 1;
    while(p <= x){
        p*=2;
        i++;
    } 
    return i-1;

}

int main()
{
    int N = 0;
    cin >> N;
    vector<int> input;
    for (int i = 0; i<N; i++)
    {
        int x;
        cin >> x;
        input.push_back(x);

    }


    for (int i = 0; i<N; i++)
    {
        cout << solve(input[i]) << endl;
    }

}