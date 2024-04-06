#include <iostream>
#include <math.h>
using std::cout;
using std::cin;


int main()
{
    int N;
    cin >> N;
    unsigned long long* defG = new unsigned long long[11];
    unsigned long long* G = new unsigned long long[N+1];
    defG[0] = 0;
    defG[1] = 1;
    defG[2] = 5;
    defG[3] = 8;
    defG[4] = 9;
    defG[5] = 10;
    defG[6] = 17;
    defG[7] = 17;
    defG[8] = 20;
    defG[9] = 24;
    defG[10] = 30;

    for (int i = 0; i<N+1;i++)
    {
        int max_cen = i < 11 ? defG[i] : 0;
        for(int j = 1; j<=std::min(10,i); j++)
        {
            int current_cen = G[j] + G[i-j];
            if(current_cen > max_cen){max_cen = current_cen;}
        }
        G[i] = max_cen;
    }
    cout << G[N];
}