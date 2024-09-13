#include <iostream>
#include <math.h>
using std::cin;
using std::cout;
struct vector2
{
    double x;
    double y;

    vector2()
    {
        x = 0;
        y = 0;
    }
    vector2(double x_, double y_){
        x = x_;
        y = y_;
    }
    vector2(int x_, int y_){
        x = (double)x_;
        y = (double)y_;
    }
    
    double length(){
        return std::sqrt(x*x + y*y);
    }

    double operator * (const vector2& another)
    {
        return x * another.x + y * another.y;
    }
    vector2 operator + (const vector2& another)
    {
        return vector2(x + another.x, y + another.y);
    }
    vector2 operator - (const vector2& another)
    {
        return vector2(x - another.x, y - another.y);
    }
    bool operator == (const vector2& another)
    {
        return x == another.x & y == another.y; 
    }
    double operator % (const vector2& another)
    {
        return (x * another.y - y * another.x);
    }
};

bool isLezhit(vector2 a1, vector2 a2, vector2 b)
{
    if (((a2 - a1) % (b-a1)) != 0){return false;}
    double k = (b - a1).x / (a2 - a1).x;
    return (0 <= k) && (k <= 1);
}

int main()
{
    vector2 a1, a2, b1, b2;
    cin >> a1.x >> a1.y >> a2.x >> a2.y >> b1.x >> b1.y >> b2.x >> b2.y;
    double c1 = ((b1-a1)%(a2-a1));
    double c2 = ((b2-a1)%(a2-a1));
    double c3 = ((a1-b1)%(b2-b1));
    double c4 = ((a2-b1)%(b2-b1));

    bool crossing = isLezhit(a1, a2, b1) || isLezhit(a1, a2, b2) 
    || isLezhit(b1, b2, a1)|| isLezhit(b1, b2, a2) || ((c1*c2 < 0) & (c3*c4 < 0));


    if (crossing)
    {
        cout << "YES\n";
    } 
    else{
        cout << "NO\n";
    }

}