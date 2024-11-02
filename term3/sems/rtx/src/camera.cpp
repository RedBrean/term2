#include <camera.h>
#include <image.h>
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

Image Camera::get_image() const {
    {
        Color** pixels = new Color*[image_width];
        for(int i = 0; i<image_width; i++)
        {
            pixels[i] = new Color[image_height];
            for(int j = 0; j<image_height; j++)
            {
                pixels[i][j] = ray_color(get_ray(i,j));
            }

        }
        return Image(image_width, image_height, pixels);
    }
}

Color ray_color(const Ray& ray) {
    Vector3D unit_direction = ray.direction_normalized();
    double a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}
