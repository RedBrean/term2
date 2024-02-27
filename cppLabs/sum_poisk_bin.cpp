#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>
#include <fstream>
#include <algorithm>

using std::vector;
using std::cout;
using std::cin;

vector<int> random_vector_sorted(int N)
{
    std::srand(std::time(0));
    vector<int> output(N);
    for(int i = 0; i<N; i++)
    {
        output[i] = std::rand();
    }
    std::sort(output.begin(), output.end());
    return output;
}

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
int bin_poisk(vector<int>vec, int value)
{
    int a, b;
    a = 0;
    b = vec.size()-1;
    int c = (a+b)/2;
    while(b-a > 1)
    {
        c = (a+b)/2;
        if(vec.at(c)==value)
        {return c;}
        else if(vec.at(c)>value)
        {b = c;}
        else {a = c;}  
    }
    if(b-a == 1 && vec.at(b) == value){return b;}
    return -1;
}
void sum_poisk(vector<int>vec, int value)
{
    for(int i = 0; i < vec.size(); i++)
    {
        for(int j = 0; j<vec.size(); j++)
        {
            if(vec.at(i) + vec.at(j) == value){
                cout << "find " << vec.at(i) << " + " << vec.at(j) << " = " << value << "\n";
                return; 
            }
        }
    }
    return;
}
void sum_poisk_bin(vector<int>vec, int value)
{
    int left, right;
    left = 0;
    right = vec.size()-1;
    while(right>=left)
    {
        if(vec.at(left) + vec.at(right) > value)
        {
            right--;
        }
        else if(vec.at(left) + vec.at(right) < value)
        {
            left++;
        }
        else
        {
                cout << "find " << vec.at(left) << " + " << vec.at(right) << " = " << value << "\n";
                return; 
        }

    }
}
int calc_time(int N)
{
    auto vec = random_vector_sorted(N);

    auto begin = std::chrono::steady_clock::now();

    sum_poisk_bin(vec, std::rand());

    auto end = std::chrono::steady_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    return time_span.count();
}

int main()
{
    int N;
    cin >> N;
    std::ofstream output_file;
    output_file.open("sum_poisk_bin.csv");
    output_file<<"N,t\n";
    for (int i = 0; i<N; i++)
    {
        auto n = (std::rand()%100000);
        auto time = calc_time(n);
        cout << "N = " << i << "\n";
        output_file << n << ", " << time << "\n";
    }

    return 0;
}