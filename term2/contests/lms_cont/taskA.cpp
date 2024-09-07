#include <iostream>

using std::cout;

void copy_4bytes_to_from(void *dst, void const *src) {
    unsigned char* p_dst = (unsigned char*)dst;
    unsigned char* p_src = (unsigned char*)src;
    for(int i = 0; i<4; i++)
    {
        p_dst[i] = p_src[i];
    }
}
void copy_fromdouble(void *dst, double const *src) {
    unsigned char* p_dst = (unsigned char*)dst;
    unsigned char* p_src = (unsigned char*)src;
    for(int i = 0; i<8; i++)
    {
        p_dst[i] = p_src[i];
    }
}

bool fake_buffer(void *buffer_begin, void *buffer_end, void const *fake, std::size_t byte_cnt) {
    unsigned char* begin = (unsigned char*)buffer_begin;
    unsigned char* end = (unsigned char*)buffer_end;
    unsigned char* p_fake = (unsigned char*)fake;
    if((((end-begin) <= (unsigned int)byte_cnt) | (end == nullptr) | (begin == nullptr) | (p_fake == nullptr) | (byte_cnt == 0)))
    { return false; }
    for(auto i = 0; i < end-begin; i++)
    {
        begin[i] = p_fake[i];
    }
    return true;
}

void reverse_ints(int *begin, int *end) {
    if(begin == nullptr){return;}
    for(int i = 0; i<(end-begin)/2; i++)
    {
        auto second = end-begin-i-1;
        int tmp = begin[i];
        begin[i] = begin[second];
        begin[second] = tmp;
    }
}

std::size_t number_of_elements(int const * const lha, int const * const rha) {
    return abs(rha-lha);
}

int main() {
    int arr[4] = {1, 2, 3, 4};
    auto const size1 = number_of_elements(&(arr[0]), &(arr[0]) + 4);
    auto const size2 = number_of_elements(&(arr[0]) + 4, &(arr[0]));
    cout << (size1 == size2);
    cout << (4 == size1);
    
    return 0;
}