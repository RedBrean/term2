#include <iostream>

typedef long long ll;

using std::cout;
using std::cin;

int main()
{
    ll N;
    cin >> N;
    ll* arr = new ll[N];

    for(ll i = 0; i<N; i++){
        ll input;
        cin >> input;
        arr[i] = input;
    }
    ll L;
    cin >> L;
    ll sum = 0;

    ll answer = N;

    for (int i = 0; i<N; i++)
    {
        sum = 0;
        for (int j = i; j<N; j++)
        {
            sum += arr[j];
            if(sum >= L && (j - i + 1) < answer){
                answer = (j - i + 1);
            }
        }
    }
    cout << answer;
    delete[] arr;
}