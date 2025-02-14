#include <geometry.h>

#include <camera.h>

#include <scene.h>

#include <iostream>

int main()
{ 
    Camera cum = Camera(2, 600, 1);

    std::cout << cum.get_ray(300, 300) << std::endl;

    Image image;
    cum.get_image(image);
    std::string path("../my_image.ppm");
    std::ofstream os(path);

    std::cout << hit_sphere(Vector3D(1, 1, -6), 2, cum.get_ray(300, 200)) << std::endl;

    image.write_image_pem(os);


    return 0;
}