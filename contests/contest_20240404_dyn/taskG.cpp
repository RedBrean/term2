#include <iostream>
#include <vector>
using std::cout;
using std::cin;
using std::vector;

int main()
{
    long N;
    cin >> N;
    long* array = new long[N]; 
    for(long i = 0; i<N; i++){
        cin >> array[i];
    }
    //двумерный массив векторов. Пиздец
    vector<long>** pod_ot_do = new vector<long>*[N];
    for(int i = 0; i<N; i++)
    {
        pod_ot_do[i] = new vector<long>[N]; 
    }

    for(long length; length<N; length++)
    {
        for(long i = 0; i<N-length; i++)
        {
            long j = i+length;


        }
    }



}