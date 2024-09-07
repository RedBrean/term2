#include <iostream>
#include <random>

using std::cout;
using std::cin;


struct Dynara{

    unsigned size;
    unsigned capacity;
    int* ptr;

    void print()
    {
        cout << "[";
        for(int i = 0; i<size; i++)
        {
            cout << ptr[i] << " "; 
        }
        cout << "]\n";
    }

    void push(int value)
    {
        if(capacity==size)
        {
            int new_capacity = capacity * 2 + 10;
            int* old_ptr = ptr;
            ptr = new int[new_capacity];
            for (int i = 0; i<capacity; i++)
            {
                ptr[i] = old_ptr[i];
            }
            delete[] old_ptr;
            capacity = new_capacity;
        }
        ptr[size] = value;
        size++;
    }

    void push_index(int value, unsigned index)
    {
        if(capacity > size & capacity > index)
        {
            for(int i = size; i>index; i--)
            {
                ptr[i] = ptr[i-1];
            }
            ptr[index] = value;
            size++;
        }
        else
        {

        }
    }
};



int main()
{
    Dynara dyn = {.size = 0, .capacity = 12, new int[12]};
    for(int i = 0; i < 10; i++)
    {
        dyn.push(std::rand()%100);
    }
    dyn.print();
    dyn.push_index(42, 2);
    dyn.print();
}