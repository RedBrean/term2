#pragma once
#include <geometry.h>
#include <image.h>
#include <vector>

class Camera {
public:
    Camera(double aspect_ratio, int image_width, double focal_length = 1.0);

    // Generates a ray from the camera through the pixel (x, y)
    Ray get_ray(int x, int y) const;

    // Generates an image by tracing rays through each pixel
    Image get_image() const;

private:
    int image_height;
    int image_width;
    double focal_length;
    double viewport_height;
    double viewport_width;

    Vector3D origin;
    Vector3D lower_left_corner;
    Vector3D horizontal;
    Vector3D vertical;
};

// Computes the color of a ray based on its direction
Color ray_color(const Ray& ray);
