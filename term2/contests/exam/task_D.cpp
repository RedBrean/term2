#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;


using std::cout;
using std::cin;
using std::vector;

ll N;
ll* arr;
ll k;


bool check_max_jump(ll max_jump)
{
    ll x = 0;
    for(ll i = 1; i<k; i++)
    {
        if (x+max_jump >= N-1){return true;}
        ll next_x = x;
        for (ll j = max_jump; j>=0; j--)
        {
            if(arr[x+j] == true)
            {
                next_x = x+j;
                break;
            }
        }
        if (next_x == x){return false;}
        else{
            x = next_x;
        }
    }
    return (x+max_jump >= N-1);
}


int main()
{

    cin >> N;
    arr = new ll[N];
        for(ll i = 0; i<N; i++){
        bool input;
        cin >> input;
        arr[i] = input;
    }

    cin >> k;
    ll answer = N-1;
    for(ll max_jump = 0; max_jump<N; max_jump++)
    {
        if(check_max_jump(max_jump)){
            answer = max_jump;
            break;
        }
    }
    cout << answer;

    delete[] arr;
}