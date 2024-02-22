#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>

using std::vector;
using std::cout;
using std::cin;

vector<int> random_vector(int N)
{
    std::srand(std::time(0));
    vector<int> output(N);
    for(int i = 0; i<N; i++)
    {
        output[i] = std::rand();
    }
    return output;
}

void vector_print(vector<int> vec)
{
    cout << "[";
    for (int i = 0; i<vec.size(); i++)
    {
        cout << vec[i]<<" ";
    }
    cout<< "]\n";
}

int poisk(vector<int> vec, int value)
{
    for (int i = 0; i<vec.size(); i++)
    {
        if(vec[i] == value){return i;}
    }
    return -1;
}

int main()
{
    int N;
    cin >> N;
    auto vec = random_vector(N);
    


}