#include <iostream>

using std::cout;
using std::cin;


int main()
{
    long N;
    cin >> N;
    long* sizes = new long[N]; 
    for(long i = 0; i<N; i++){
        cin >> sizes[i];
    }

    long** sloznost_ot_do = new long*[N];
    for(long i = 0; i<N; i++){
        sloznost_ot_do[i] = new long[N];
    }
    
    for(long length; length<N; length++)
    {
        for(long i = 0; i<N-length; i++)
        {
            long j = i+length;
            if((j-i)<2){
                sloznost_ot_do[i][j] = 0;
                continue;
            }
            
            long min_sloznost = -1; 
            for(int k = i+1; k<j; k++)
            {
                long last_mult, left_mult, right_mult;
                last_mult = sizes[i]*sizes[k]*sizes[j];
                left_mult = sloznost_ot_do[i][k];
                right_mult = sloznost_ot_do[k][j];
                long current_sloznost = last_mult + left_mult + right_mult;
                if(current_sloznost < min_sloznost || min_sloznost==-1)
                {
                    min_sloznost = current_sloznost;
                }
            }
            sloznost_ot_do[i][j] = min_sloznost;
            //cout << "i = " << i <<", j = " << j << " sloznost = " << min_sloznost <<"\n";
        }
    }

    // for(int i = 0; i<N; i++)
    // {
    //     for(int j=i; j<N; j++)
    //     {
    //         cout << sloznost_ot_do[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    
    cout<<sloznost_ot_do[0][N-1];

}