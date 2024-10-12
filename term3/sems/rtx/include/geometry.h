#pragma once
#include <ostream>

class Vector3D {
public:
    // Конструкторы
    Vector3D();
    Vector3D(double x, double y, double z);

    // Операторы присваивания
    Vector3D& operator+= (Vector3D& second);
    Vector3D& operator-= (Vector3D& second);
    Vector3D& operator*= (double value);
    Vector3D& operator/= (double value);

    //Методы
    double length();
    double length_squared();

    // Дружественный оператор для вывода
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& point);

    // Переменные-члены
    double x, y, z;
};

Vector3D operator+(const Vector3D& vec, const Vector3D& second);
Vector3D operator-(const Vector3D& vec, const Vector3D& second);
Vector3D operator* (const double& value, const Vector3D& vec);
Vector3D operator*(const Vector3D& vec, const double& value);
Vector3D operator/(const Vector3D& vec, const double& value);

/*
Скалярное произведение*/
double operator*(const Vector3D&  vec, const Vector3D&  vec2);
/*
    Векторное произведение*/
Vector3D operator||(const Vector3D&  vec, const Vector3D&  vec2);

class Ray{
    public: 
        Vector3D at(double t);
        Ray(const Vector3D& A, const Vector3D& b);
    private:
        Vector3D A;
        Vector3D b;
}