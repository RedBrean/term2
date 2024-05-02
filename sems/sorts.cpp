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
    if(end - start <= 1){
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
        if(vec[i]<pivot){i++;}
        else if(vec[j]>pivot){j--;}
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

void merge_sort(vector<int> &vec, int start = 0, int end = -1)
{
    if(end == -1) {end = vec.size();}
    if(end - start <= 0){return;}
    else if(end - start <= 4) {
        selection_sort(vec, start, end);
        return;
    }
    auto slice_index = (start+end)/2;
    merge_sort(vec, start, slice_index);
    merge_sort(vec, slice_index, end);
    vector<int> buffer(end-start);
    for(int i = start; i<end; i++)
    {
        buffer[i-start] = vec[i];
    }

    int l,r;
    l = 0;
    r = slice_index-start;
    for(int i = start; i<end; i++)
    {
        if(l<slice_index-start && buffer[l]<=buffer[r])
        {
            vec[i] = buffer[l];
            l++;
//            if(l == slice_index-start) {r++;}
        }
        else if(r<end-start && buffer[l]>=buffer[r])
        {
            vec[i] = buffer[r];
            r++;
//          if(r == end-start) {l++;}
        }
    }



}

struct Heap
{
    vector<int> *vec_pointer;
    int size;
    vector<int>& vec()
    {return *vec_pointer;} 
};

void add_value_to_heap(Heap& heap, int value)
{
    if(heap.vec().size() == heap.size)
    {
        int new_size = 1;
        while(new_size <= heap.size)
        {
            new_size*=2;
            new_size++;
        }
        heap.vec().resize(new_size);
    }
    heap.vec()[heap.size] = value;
    int i = heap.size;
    while(heap.vec()[i] > heap.vec()[(i-1)/2] & i > 0)
    {
        std::swap(heap.vec()[i], heap.vec()[(i-1)/2]);
        i = (i-1)/2;
    }
    heap.size++;
}

void heapify(Heap &heap)
{
   int old_size = heap.size;
   heap.size = 0;
   for(int i = 0; i < old_size; i++)
   {
        add_value_to_heap(heap, heap.vec()[i]);
   }
}

void heap_sort(vector<int> &vec)
{
    Heap heap;
    heap.size = vec.size();
    heap.vec_pointer = &vec;
    heapify(heap);
    for(int i = vec.size()-1; i>=0; i--)
    {
        std::swap(vec[0], vec[i]);
        heap.size = heap.size-1;
        heapify(heap);
    }

}

int main()
{
    auto vec = random_vector(10);
    vector_print(vec);
    heap_sort(vec);
    vector_print(vec);
}