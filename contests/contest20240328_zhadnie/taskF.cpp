#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;

int N;
vector<int> S, F;

int find_first_and_smalles_after(int after){
    int result = -1;
    int last_end = 1000;
    for(int  i = 0; i!=N; i++)
    {
        if(S[i] >= after)
        {
            if(F[i] < last_end)
            {
                result = i;
                last_end = F[i];
            }
        }
    }
    return result;
}

int main()
{

    cin >> N;
    S.resize(N);
    F.resize(N);
    for(int i = 0; i!=N; i++)
    {
        cin >> S[i] >> F[i];
    }
    int answer = 0;
    int last_lec = find_first_and_smalles_after(0);
    while(last_lec != -1)
    {
        last_lec = find_first_and_smalles_after(F[last_lec]);
        answer++;
    }
    cout << answer;

}
