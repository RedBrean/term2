#pragma once
#include <ostream>
class Vector3D{
    public:
        Vector3D() : x(0), y(0), z(0){};
        Vector3D(double x, double y, double z) : x(x), y(y), z(z) {};
        
        Vector3D& operator+= (Vector3D& second)
        {
            this->x += second.x;
            this->y += second.y;
            this->z += second.z;
            return *this;
        }
        Vector3D& operator-= (Vector3D& second)
        {
            this->x -= second.x;
            this->y -= second.y;
            this->z -= second.z;
            return *this;
        }
        Vector3D& operator*= (double value)
        {
            this->x *= value;
            this->y *= value;
            this->z *= value;
            return *this;
        }
        Vector3D& operator/= (double value)
        {
            this->x /= value;
            this->y /= value;
            this->z /= value;
            return *this;
        }
        
        friend std::ostream& operator<<(std::ostream& os, const Vector3D& point);
    public:
        double x, y, z;

};

inline Vector3D operator+(const Vector3D& vec, const Vector3D& second){
    return Vector3D(vec.x + second.y, vec.y + second.y, vec.z + second.z);
}
inline Vector3D operator-(const Vector3D&  vec, const Vector3D&  second){
    return Vector3D(vec.x - second.y, vec.y - second.y, vec.z - second.z);
}
inline Vector3D operator* (const double&  value, const Vector3D&  vec){
    return Vector3D(vec.x*value, vec.y*value, vec.z*value);
}
inline Vector3D operator*(const Vector3D&  vec, const double&  value){
    return Vector3D(vec.x*value, vec.y*value, vec.z*value);
}
inline Vector3D operator/(const Vector3D&  vec, const double& value){
    return Vector3D(vec.x/value, vec.y/value, vec.z/value);
}

/*
Скалярное произведение*/
inline Vector3D operator*(const Vector3D&  vec, const Vector3D&  vec2){
    return Vector3D(vec.x*vec2.x, vec.y*vec2.y, vec.z*vec2.z);
}
/*
    Векторное произведение*/
inline Vector3D operator||(const Vector3D&  vec, const Vector3D&  vec2){
    return Vector3D(vec.y*vec2.z - vec.z*vec2.y, vec.z*vec2.x - vec.x*vec2.z, vec.x*vec2.y - vec.y*vec2.x);
}
