#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
using std::cout;
using std::cin;
using std::vector;



int main()
{
    int N;
    cin >> N;
    vector<vector<int>> sresi = vector<vector<int>>(N+1);
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
        int max_cen = 0;
        if(i < 11){
            max_cen = defG[i];
            vector<int> tupoi_srez;
            tupoi_srez.push_back(i);
            sresi[i] = tupoi_srez;
        }
        int max_j = -1;
        for(int j = 1; j<=std::min(10,i); j++)
        {
            int current_cen = G[j] + G[i-j];
            if(current_cen > max_cen){
                max_cen = current_cen;
                max_j = j;
            }
        }
        G[i] = max_cen;
        if(max_j != -1)
        {
            vector<int> new_sres;
            for(int k = 0; k<sresi[max_j].size(); k++)
            {
                new_sres.push_back(sresi[max_j][k]);
            }
            for(int k = 0; k<sresi[i-max_j].size(); k++)
            {
                new_sres.push_back(sresi[i-max_j][k]);
            }
            sresi[i] = new_sres;
        }

    }
    vector<int> answer = sresi[N];
    std::sort(answer.begin(), answer.end());
    for(int i = 0; i<answer.size();i++)
    {
        cout << answer[i] << " ";
    }
    
}