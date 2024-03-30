#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;



int main()
{
    int k, M;
    cin >> k;
    int* a = new int[k];
    for(int i = 0; i != k; i++)
    {
        cin >> a[i]; 
    } 
    cin >> M;
    int answer = 0;
    for(int i = k-1; i >= 0; i--)
    {
        answer += M/a[i];
        M = M%a[i];
    } 
    cout << answer;

}
