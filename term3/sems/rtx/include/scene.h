#pragma once
#include <geometry.h>


bool hit_sphere(const Vector3D& center, double radius, const Ray& r) {
    Vector3D oc = center - r.begin();
    auto a = r.direction() * r.direction();
    auto b = -2.0 * (r.direction() * oc);
    auto c = oc * oc - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

