#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file("1.bin", std::ios::binary);



    std::ofstream out_file("out.md");

    char* buffer_ptr = new char;

    for(int j=37;j!=38;j++)
    {
    std::cout << "\n j=" << j << std::endl;
        file.seekg(0, std::ios::beg);
        while(file)
        {
            file.read(buffer_ptr, 1);
            

            char out = static_cast<char>(((static_cast<int>(*buffer_ptr) - j) + 256) % 128);

            // std::cout << (static_cast<int>(*buffer_ptr)) << " >> " << ((static_cast<int>(*buffer_ptr) -j) + 256) % 128 << " >> " << out << std::endl;
            out_file << out;
            // std::cout << out;

        }
    std::cout << std::endl;
    }
    
    delete buffer_ptr;


    file.close();
    out_file.close();

    return 0;
}