#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <iomanip>

using std::vector;
using std::cout;
using std::cin;


int random_int()
{
    return std::rand()%100;
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

struct Heap
{
    vector<int> vec;
    int size;
};


void add_value_to_heap(Heap& heap, int value)
{
    if(heap.vec.size() == heap.size)
    {
        int new_size = 1;
        while(new_size <= heap.size)
        {
            new_size*=2;
            new_size++;
        }
        heap.vec.resize(new_size);
    }
    heap.vec[heap.size] = value;
    int i = heap.size;
    while(heap.vec[i] > heap.vec[(i-1)/2] & i > 0)
    {
        std::swap(heap.vec[i], heap.vec[(i-1)/2]);
        i = (i-1)/2;
    }
    heap.size++;
}

void make_heap(Heap &heap)
{
   int old_size = heap.size;
   heap.size = 0;
   for(int i = 0; i < old_size; i++)
   {
        add_value_to_heap(heap, heap.vec[i]);
   }
}


void heap_print(Heap heap) {
    const int levels = std::log2(heap.size) + 1;
 
    std::vector<int> offsets(levels + 1, 0);
    for (int i = offsets.size() - 2; i >= 0; --i)
        offsets[i] = 2 * offsets[i+1] + 2;
 
    auto printOffset = [](int offset) {
        for (int i = 0; i < offset; ++i) std::cout << ' ';
    };
 
    for (int level = 0; level < levels; ++level)
    {
        printOffset(offsets[level]);
        for(int j = pow(2, level) - 1; j  < std::min(heap.size, int(pow(2, level + 1)) - 1); ++j) {
            std::cout << std::setw(2) << heap.vec[j];
            if (level) printOffset(offsets[level - 1]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
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


int main()
{
    Heap heap;
    heap.vec = random_vector(12);
    heap.size = 12;
    heap_print(heap);
    make_heap(heap);
    cout << "сортируем......\n";
    heap_print(heap);

}




