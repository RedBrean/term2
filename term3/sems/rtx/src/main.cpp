#include <geometry.h>

#include <camera.h>

#include <iostream>

int main()
{ 
    Camera cum = Camera(2, 400, 1);

    std::cout << cum.get_ray(100, 150);

    return 0;
}