#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file("2.bin", std::ios::binary);

    std::ofstream out_file("out_2.md");

    char* buffer_ptr = new char;

    int* int_buffer = new int;
    file.read(reinterpret_cast<char*>(int_buffer), 4);
    std::cout << *int_buffer << std::endl;

    while(file)
    {
        file.read(buffer_ptr, 1);
    }

    delete buffer_ptr;


    file.close();
    out_file.close();

    return 0;
}