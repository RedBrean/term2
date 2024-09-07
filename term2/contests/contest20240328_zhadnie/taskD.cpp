#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;



int main()
{
    long long S, N;
    cin >> S >> N;
    vector<long long> D(N);
    for(int i = 0; i!=N; i++)
    {
        cin >> D[i];
    }
    std::sort(D.begin(), D.end());
    int answer = 0;
    for(int i = 0; i!=N; i++)
    {
        if(S >= D[i]){
            answer++;
            S-=D[i];
        }
        else{
            break;
        }
    }
    cout << answer;

}
