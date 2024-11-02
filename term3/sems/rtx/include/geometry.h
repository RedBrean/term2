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
    double length() const;
    double length_squared() const;
    Vector3D normalize() const;

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
        Vector3D at (double t) const;
        Vector3D begin() const;
        Vector3D direction() const;
        Vector3D direction_normalized() const;
        
        /*A - начало луча, b - направление*/
        Ray(const Vector3D& A, const Vector3D& b); 
    
        // Дружественный оператор для вывода
        friend std::ostream& operator<<(std::ostream& os, const Ray& ray);
 

    private:
        Vector3D A;
        Vector3D b;
};


using Color = Vector3D;
