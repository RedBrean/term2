#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;


using std::cout;
using std::cin;
using std::vector;

vector<ll> length(34);

ll find(ll i, ll depth)
{
    if(i == 0){return depth;}
    return find((i-1) % length[depth-1], depth-1);
}


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
    length[0] = 0;
    for(ll i = 1; i<34; i++){
        length[i] = length[i-1]*2 + 1;
    }

    for(ll index = 0; index<N; index++){
        cout << find(arr[index], 33) << " ";
    }

}