#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;



int main()
{
    int N, k, S;
    cin >> N >> k >> S;;
    vector<int> D(S+1);
    for(int i = 0; i!=S; i++)
    {
        cin >> D[i];
    }
    D[S] = N;
    int lastStop = 0;
    int answer = 0;
    for (int i = 0; i!=S; i++)
    {
        if((D[i] - lastStop) <= k & (D[i+1] - lastStop) > k)
        {
            lastStop = D[i];
            answer++;
        }
    }
    if(N - lastStop > k){cout<<-1;}
    else{cout << answer;}
}
