#include <iostream>
#include <vector>
#include <limits>
#include <ctime>

using std::vector;
using std::cout;
using std::cin;

int random_int()
{
    return std::rand()%100;
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

int max(vector<int> &vec, int start = 0, int end = -1)
{
    if(end == -1){end = vec.size();}
    int result = vec[start];
    for (int  i = start; i<end; i++)
    {
        if(vec[i] > result){result = vec[i];}
    }
    return result;
}

int min(vector<int> &vec, int start = 0, int end = -1)
{
    if(end == -1){end = vec.size();}
    int result = vec[start];
    for (int  i = start; i<end; i++)
    {
        if(vec[i] < result){result = vec[i];}
    }
    return result;
}

int max_index(vector<int> &vec, int start = 0, int end = -1)
{
    if(end == -1){end = vec.size();}
    int value = vec[start];
    int index = 0;
    for (int  i = start; i<end; i++)
    {
        if(vec[i] > value){
            value = vec[i];
            index = i;
        }
    }
    return index;
}

int min_index(vector<int> &vec, int start = 0, int end = -1)
{
    if(end == -1){end = vec.size();}
    int value = vec[start];
    int index = start;
    for (int  i = start; i<end; i++)
    {
        if(vec[i] < value){
            value = vec[i];
            index = i;
        }
    }
    return index;
}

vector<int> random_vector(int N)
{
    std::srand(std::time(0));
    vector<int> output(N);
    for(int i = 0; i<N; i++)
    {
        output[i] = random_int();
    }
    return output;
}

void selection_sort(vector<int> &vec, int start = 0, int end = -1) 
/*сортирует [start, end)*/
{
    if(end == -1){end  = vec.size();}
    for (int sorted_index = start; sorted_index<end; sorted_index++)
    {
        std::swap(vec[sorted_index], vec[min_index(vec, sorted_index, end)]);
    }
}

void insertion_sort(vector<int> &vec)
{
    for (int sorted_index = 0; sorted_index<vec.size()-1; sorted_index++)
    {
        int i = sorted_index;
        while(i>0 & vec[i-1]>vec[i])
        {
            std::swap(vec[i-1], vec[i]);
            i--;
        }
    }
}

void bubble_sort(vector<int> &vec)
{
    for(int i = vec.size()-1; i >= 0; i--)
    {
        for(int j = 0; j<i; j++)
        {
            if(vec[j]>vec[j+1]){std::swap(vec[j], vec[j+1]);}
        }
    }
}

void quick_sort(vector<int> &vec, int start = 0, int end = -1)
{
    if(end == -1){end  = vec.size();}
    if(end - start <= 0){
        return;
    }
    else if(end - start < 4){
        selection_sort(vec, start, end);

        return;
    }
    auto pivot_index = (start+end)/2;
    auto pivot = vec[pivot_index];
    auto i = start;
    if(end <= 0){return;}
    auto j = end-1;
    
    while(i <= j)
    {
        if(vec[i]<=pivot){i++;}
        else if(vec[j]>=pivot){j--;}
        else{
            std::swap(vec[i], vec[j]);
            i++;
            j--;
        }
    }
    auto mid = i;
    quick_sort(vec, start, mid);
    quick_sort(vec, mid, end);
    return;
}

int main()
{
    auto vec = random_vector(10);
    vector_print(vec);
    quick_sort(vec);
    vector_print(vec);
}