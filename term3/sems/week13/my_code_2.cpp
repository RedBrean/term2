#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("2.bin", std::ios::binary);

    std::ofstream out_file("out_2.md");

    u_int32_t length;
    file.read()

    file.close();
    out_file.close();

    return 0;
}