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
using std::string;

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

int random_value(bool smeshenie = false)
{
    if(smeshenie)
    {return std::rand()%1000;}
    else
    {return std::rand()%100000;}
}

vector<int> random_vector(int N)
{
    std::srand(std::time(0));
    vector<int> output(N);
    for(int i = 0; i<N; i++)
    {
        output[i] = random_value();
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

int poisk_A(vector<int> &vec, int value)
{
    for (int i = 0; i<vec.size(); i++)
    {
        if(vec.at(i) == value){
        if(i != 0) {std::swap(vec[0], vec[i]);}    
        return i;
        }
    }
    return -1;
}
int poisk_B(vector<int> &vec, int value)
{
    for (int i = 0; i<vec.size(); i++)
    {
        if(vec[i] == value){
        if(i != 0) {std::swap(vec[i-1], vec[i]);}    
        return i;
        }
    }
    return -1;
}
int poisk_C(vector<int> &vec, vector<int> &vesa, int value)
{
    for (int i = 0; i<vec.size(); i++)
    {
        if(vec[i] == value){
        vesa.at(i) += 1;
        if((i != 0) && vesa.at(i-1) < vesa.at(i))
        {
            std::swap(vec.at(i), vec.at(i-1));
        }    
        return i;
        }
    }
    return -1;
}
int calc_times(int size, int N = 10000, char bookva = 'A', bool ravnomerno = true)
{
    auto vec = random_vector(size);
    vector<int> vesa(size);
    for(int i = 0; i<size; i++)
    {
        vesa.at(i) = 0;
    }

    std::chrono::microseconds sum_time = std::chrono::microseconds();
    for(int i = 0; i<N; i++)
    {
        int value = random_value(!ravnomerno);
        auto begin = std::chrono::steady_clock::now();
        if(bookva == 'A')
        {       
            poisk_A(vec, value);
        }
        else if(bookva == 'B')
        {
            poisk_B(vec, value);
        }
        else if(bookva == 'C')
        {
            poisk_C(vec, vesa, value);
        }

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        sum_time += time_span;
    }
    return (sum_time/N).count();

}
void calc_settings(int N = 20000, char bookva = 'A', bool ravnomerno = true)
{
    std::ofstream file;
    file.open("smart_poisk.csv", std::ios_base::app);
    auto n = (std::rand()%100000 + 200);
    auto time = calc_times(n, N, bookva, ravnomerno);
    file << n << "," << bookva << "," << ravnomerno << "," << time << "\n";
    file.close();
}

int main()
{
    int N = 20;
    std::ofstream output_file;
    output_file.open("smart_poisk.csv");
    output_file<<"N,bookva,ravnomerno,t\n";
    output_file.close();
    for (int i = 0; i<N; i++)
    {
        //сорри за индусский код
        calc_settings(1000, 'A', true);
        calc_settings(1000, 'B', true);
        calc_settings(1000, 'C', true);
        calc_settings(1000, 'A', false);
        calc_settings(1000, 'B', false);
        calc_settings(1000, 'C', false);
        cout << "N = " << i << "\n";
    }


    return 0;
}