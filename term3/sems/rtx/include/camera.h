#pragma once
#include "geometry.h"

using Color = Vector3D;

class Camera {
public:
    Camera(double aspect_ratio, int image_width, double focal_length = 1)
    {
        this->image_width = image_width;
        // Рассчитываем размеры изображения и окна просмотра
        image_height = int(image_width / aspect_ratio);
        viewport_height = 2.0;
        viewport_width = viewport_height * aspect_ratio;

        // Центр камеры и расчет векторов для видового экрана
        origin = Vector3D(0, 0, 0);
        horizontal = Vector3D(viewport_width, 0, 0);
        vertical = Vector3D(0, viewport_height, 0);

        lower_left_corner = origin - horizontal / 2 - vertical / 2 + Vector3D(0, 0, -focal_length);
    }

    // Метод для генерации луча от камеры через пиксель (u,v)
    Ray get_ray(int x, int y) const {
        return Ray(origin, lower_left_corner + x * horizontal / image_width + y * vertical / image_height - origin);
    }
public:
    int image_height;
    int image_width;
private:
    Vector3D origin;
    Vector3D lower_left_corner;
    Vector3D horizontal;
    Vector3D vertical;
    double focal_length;
    double viewport_height;
    double viewport_width;

};
