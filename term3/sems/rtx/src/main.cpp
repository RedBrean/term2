#include <geometry.h>
#include <iostream>

int main()
{   
    Vector3D vec = Vector3D(1, 2, 3);
    Vector3D vec2 = Vector3D(1,-2, 3);
    auto vec3 = vec||vec2;
    std::cout << ((vec3||vec)*3).length()  << "\n";
    return 0;
}