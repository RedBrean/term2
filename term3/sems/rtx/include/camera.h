#pragma once
#include <geometry.h>
#include <image.h>
#include <vector>

#include <scene.h>

class Camera {
public:
    Camera(double aspect_ratio, int image_width, double focal_length = 1.0);

    // Generates a ray from the camera through the pixel (x, y)
    Ray get_ray(int x, int y) const;

    // Generates an image by tracing rays through each pixel
    void get_image(Image& image) const;

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
Color ray_color(const Ray& ray) {
    // if(!hit_sphere(Vector3D(1, 1, -1), 1, ray)){return Vector3D(0,0,0);}
    Vector3D unit_direction = ray.direction_normalized();
    double a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}
