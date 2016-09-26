#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <iostream>
#include <vector>

// TODO:
// release complex operators (+=, -=, etc)

class Matrix;

template <typename T> struct Vec2
{
    union {
        struct {T x, y;};
        T raw[2];
    };

    Vec2<T>() : x(T()), y(T()) {}
    Vec2<T>(T _x, T _y) : x(_x), y(_y) {}

    Vec2<T> operator+ (const Vec2<T> &V) const
    {
        return Vec2<T> (x + V.x, y + V.y);
    }

    Vec2<T> operator- (const Vec2<T> &V) const
    {
        return Vec2<T> (x - V.x, y - V.y);
    }

    Vec2<T> operator* (double f) const
    {
        return Vec2<T> (x*f, y*f);
    }
    T& operator[](const int i)
    {
        if (i <= 0)
            return x;
        else
            return y;
    }
    template <typename > friend std::ostream& operator <<(std::ostream& s, Vec2<T>& v);
};

template <typename T> std::ostream& operator <<(std::ostream& s, Vec2<T>& v)
{
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <typename T> struct Vec3
{
    union{
        struct {T x, y, z;};
        T raw[3];
    };

    Vec3<T>() : x(T()), y(T()), z(T()) {}
    Vec3<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Vec3<T>(Matrix m);

    template<typename U> Vec3<T> (const Vec3<U> &v);

    Vec3<T> operator^ (const Vec3<T> &v) const{
        return Vec3<T> (y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    Vec3<T> operator+ (const Vec3<T> &v) const{
        return Vec3<T> (x + v.x, y + v.y, z + v.z);
    }
    Vec3<T>& operator+= (const Vec3<T>& v){
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }
    Vec3<T> operator- (const Vec3<T> &v) const{
        return Vec3<T> (x - v.x, y - v.y, z - v.z);
    }
    Vec3<T>& operator-= (const Vec3<T>& v){
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this;
    }
    Vec3<T> operator* (double f) const{
        return Vec3<T> (x*f, y*f, z*f);
    }
    Vec3<T>& operator*= (double f){
        this->x *= f;
        this->y *= f;
        this->z *= f;
        return *this;
    }
    T operator* (const Vec3<T> &v) const{
        return x*v.x + y*v.y + z*v.z;
    }
    T& operator[](const int i) {
        if (i <= 0)
            return x;
        else if (i == 1)
            return y;
        else
            return z;
    }
    double length() const{
        return std::sqrt(x*x + y*y + z*z);
    }
    Vec3<T>& normalize (T l = 1){
        *this = (*this)*(l/length());
        return *this;
    }
    void setX(double value){
        x = value;
    }
    void setY(double value){
        y = value;
    }
    void setZ(double value){
        z = value;
    }

    template <typename > friend std::ostream& operator << (std::ostream& s, Vec3<T>& v);
};

template <typename T> std::ostream& operator << (std::ostream& s, Vec3<T>& v){
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}


typedef Vec2<double> Vec2d;
typedef Vec3<double> Vec3d;
typedef Vec2<int> Vec2i;
typedef Vec3<int> Vec3i;



#endif // GEOMETRY_H
