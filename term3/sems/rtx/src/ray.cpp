#include <geometry.h>

Vector3D Ray::at(double t) const
{
    return A + b*t;
}
Ray::Ray(const Vector3D& A, const Vector3D& b){
    this->A = A;
    this->b = b;
}
Vector3D Ray::begin() const {
    return A;
}
Vector3D Ray::direction() const{
    return b;
}

Vector3D Ray::direction_normalized() const
{
    return b.normalize();
}
std::ostream& operator<<(std::ostream& os, const Ray& ray)
{
    os << "Begin: " << ray.A << ", Direction: " << ray.b;
}