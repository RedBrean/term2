#include <iostream>
#include <ostream>
#include <vec3.h>

std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z <<")";
    return os;
}
