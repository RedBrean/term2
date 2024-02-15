#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, b;
    cin >> n >> b;
    vector<int> res;

    while(n > 0)
    {   
        res.push_back(n%b);
        n = n/b;
    }
    
    bool fl = false;
    for(int i = res.capacity()-1; i>=0; i--)
    {
        if(res[i] != 0) {fl = true;}
        if(fl){cout << res[i];}
    }
    if(!fl){cout << 0;}
}