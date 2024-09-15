#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;

using std::cout;
using std::cin;
using std::vector;
int main()
{
    ll N;
    cin >> N;
    vector<ll> arr(N);
    for(ll i = 0; i<N; i++){
        ll input;
        cin >> input;
        arr[i] = input;
    }
    std::sort(arr.begin(), arr.end());

    for(ll i = N-1; i>=0; i--){
        cout << arr[i];
    }

}