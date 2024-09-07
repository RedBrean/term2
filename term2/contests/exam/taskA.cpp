#include <iostream>

typedef long long ll;

using std::cout;
using std::cin;

int main()
{
    ll N;
    cin >> N;
    ll* arr = new ll[N];
    ll all_sum = 0;
    for(ll i = 0; i<N; i++){
        ll input;
        cin >> input;
        arr[i] = input;
        all_sum += input;
    }
    ll c_sum = 0;
    ll answer = N;
    for(ll i = 0; i<N; i++)
    {
        if(c_sum*2 == all_sum){
            answer = i;
            break;
        }
        c_sum += arr[i];
    }
    cout << answer;
    delete[] arr;
}