class Vector3D{
    public:
        Vector3D() : x(0), y(0), z(0);
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

    public:
        double x, y, z;
};
Vector3D operator+(Vector3D vec, Vector3D second){
    return Vector3D(vec.x + second.y, vec.y + second.y, vec.z + second.z);
}
Vector3D operator-(Vector3D vec, Vector3D second){
    return Vector3D(vec.x - second.y, vec.y - second.y, vec.z - second.z);
}
Vector3D operator*(double value, Vector3D vec){
    return Vector3D(vec.x*value, vec.y*value, vec.z*value);
}
Vector3D operator*(Vector3D vec, double value){
    return Vector3D(vec.x*value, vec.y*value, vec.z*value);
}
Vector3D operator/(Vector3D vec, double value){
    return Vector3D(vec.x/value, vec.y/value, vec.z/value);
}
/*
    Скалярное произведение

*/
Vector3D operator*(Vector3D vec, Vector3D vec2){
    return Vector3D(vec.x*vec2.x, vec.y*vec2.y, vec.z*vec2.z);
}
/*
    Векторное произведение
*/
Vector3D operator||(Vector3D vec, Vector3D vec2){
    return Vector3D(vec.y*vec2.z - vec.z*vec2.y, vec.z*vec2.x - vec.x*vec2.z, vec.x*vec2.y - vec.y*vec2.x);
}
