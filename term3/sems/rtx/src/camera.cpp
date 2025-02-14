#include <camera.h>
#include <image.h>
#include <geometry.h>

#include <vector>

Camera::Camera(double aspect_ratio, int image_width, double focal_length)
    : image_width(image_width), focal_length(focal_length), origin(Vector3D(0, 0, 0)),
      viewport_height(2.0), viewport_width(viewport_height * aspect_ratio)
{
    image_height = static_cast<int>(image_width / aspect_ratio);
    horizontal = Vector3D(viewport_width, 0, 0);
    vertical = Vector3D(0, viewport_height, 0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 + Vector3D(0, 0, -focal_length);
}

Ray Camera::get_ray(int x, int y) const {
    return Ray(origin, lower_left_corner + (x + 0.5) * horizontal / image_width + 
               (y + 0.5) * vertical / image_height - origin);
}

void Camera::get_image(Image& image) const {
    {
        std::vector<std::vector<Color>> pixels(image_width);
        for(int i = 0; i<image_width; i++)
        {
            pixels[i] = std::vector<Color>(image_height);
            for(int j = 0; j<image_height; j++)
            {
                pixels[i][j] = ray_color(get_ray(i,j));
            }

        }
        image.width = image_width;
        image.height = image_height;
        image.pixels = pixels;
    }
}



