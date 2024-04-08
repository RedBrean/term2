#include <iostream>
using std::cin; 
using std::cout;

int* merge_sorted(int const *first_begin, int const *first_end, int const *second_begin, int const *second_end)
{
    if(first_end == first_begin && second_begin == second_end){return nullptr;}
    

}
#include <cstddef>
#include <cassert>


int main() {
    int const arr1[3] = {1, 2, 3};
    int const arr2[3] = {0, 2, 4};
    int const expected[6] = {0, 1, 2, 2, 3, 4};
    int *merged = merge_sorted(arr1, arr1 + 3, arr2, arr2 + 3);
    assert(nullptr != merged); // expect no memory error
    for (std::size_t idx = 0u; idx != 6u; ++idx)
        assert(merged[idx] == expected[idx]);
    delete[] merged;
    return 0;
}