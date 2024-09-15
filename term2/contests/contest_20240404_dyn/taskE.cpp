#include <iostream>
#include <math.h>
#include <string>
using std::cout;
using std::cin;
using std::string;

int main() {
    string s1;
    string s2;
    std::cin >> s1 >> s2;
    int m,n;
    m = s1.length();
    n = s2.length();
    int** table = new int*[m];
    for(int i = 0; i<m; i++)
    {
        table[i] = new int[n];
    }

    for(int i = 0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            int left, up, diag;
            left = i != 0 ? table[i-1][j] : 0;
            up = j != 0 ? table[i][j-1] : 0;
            diag = i != 0 & j != 0 ? table[i-1][j-1] : 0; 
            table[i][j] = std::max(std::max(left, up), diag + (s1[i]==s2[j]));
        }
    }
    // for(int i = 0; i<m; i++)
    // {
    //     for(int j=0; j<n; j++)
    //     {
    //         cout << table[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    int obsh = table[m-1][n-1];
    cout << obsh*100/std::min(m,n);

}