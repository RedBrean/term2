#include <geometry.h>

#include <camera.h>

#include <iostream>

int main()
{ 
    Camera cum = Camera(2, 400, 1);

    std::cout << cum.get_ray(100, 150);

    Image image = cum.get_image();
    std::string path("my_image.ppm");
    std::ofstream os(path);
    image.write_image_pem(os);


    return 0;
}