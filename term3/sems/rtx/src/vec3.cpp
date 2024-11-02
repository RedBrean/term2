#include <iostream>
#include <ostream>
#include <geometry.h>
#include <cmath>

Vector3D::Vector3D() : x(0), y(0), z(0) {}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

double Vector3D::length_squared() const
{ return x*x + y*y + z*z; }
double Vector3D::length() const
{ return std::sqrt(length_squared()); }
Vector3D Vector3D::normalize() const
{ return *this/this->length(); }

// Операторы присваивания
Vector3D& Vector3D::operator+= (Vector3D& second) {
    this->x += second.x;
    this->y += second.y;
    this->z += second.z;
    return *this;
}

Vector3D& Vector3D::operator-= (Vector3D& second) {
    this->x -= second.x;
    this->y -= second.y;
    this->z -= second.z;
    return *this;
}

Vector3D& Vector3D::operator*= (double value) {
    this->x *= value;
    this->y *= value;
    this->z *= value;
    return *this;
}

Vector3D& Vector3D::operator/= (double value) {
    this->x /= value;
    this->y /= value;
    this->z /= value;
    return *this;
}

//оператор вывода
std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z <<")";
    return os;
}


Vector3D operator+(const Vector3D& vec, const Vector3D& second) {
    return Vector3D(vec.x + second.x, vec.y + second.y, vec.z + second.z);
}

Vector3D operator-(const Vector3D& vec, const Vector3D& second) {
    return Vector3D(vec.x - second.x, vec.y - second.y, vec.z - second.z);
}

Vector3D operator* (const double& value, const Vector3D& vec) {
    return Vector3D(vec.x * value, vec.y * value, vec.z * value);
}

Vector3D operator*(const Vector3D& vec, const double& value) {
    return Vector3D(vec.x * value, vec.y * value, vec.z * value);
}

Vector3D operator/(const Vector3D& vec, const double& value) {
    return Vector3D(vec.x / value, vec.y / value, vec.z / value);
}

double operator*(const Vector3D&  vec, const Vector3D&  vec2){
    return (vec.x*vec2.x + vec.y*vec2.y, vec.z*vec2.z);
}

Vector3D operator||(const Vector3D&  vec, const Vector3D&  vec2){
    return Vector3D(vec.y*vec2.z - vec.z*vec2.y, vec.z*vec2.x - vec.x*vec2.z, vec.x*vec2.y - vec.y*vec2.x);
}
