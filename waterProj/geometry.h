#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <iostream>
#include <vector>

// TODO:
// release complex operators (+=, -=, etc)

//class Matrix;

/*template <typename T> struct Vec2
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
        *this *= l/length();
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
typedef Vec3<int> Vec3i;*/


/*################################################*/
/*           NEW VERSION OF GEOMETRY              */
/*################################################*/


template <size_t dim, typename T>
struct vec{
    vec() {
        for (size_t i = dim; i--; _data[i] = T());
    }

    T& operator[] (const size_t i){
        // exc
        return _data[i];
    }

    const T& operator[] (const size_t i) const{
        return _data[i];
    }

    T operator* (const vec<dim, T>& rhs){
        T ret = T();
        for (size_t i = dim; i--; ret += _data[i] * rhs._data[i]);
        return ret;
    }

    vec<dim, T> operator+ (const vec<dim, T>& rhs){
        vec<dim, T> res;
        for (size_t i = dim; i--; res._data[i] = this->_data[i] + rhs._data[i]);
        return res;
    }

    vec<dim, T>& operator+= (const vec<dim, T>& rhs){
        for (size_t i = dim; i--; this->_data[i] += rhs._data[i]);
        return *this;
    }

    vec<dim, T> operator- (const vec<dim, T>& rhs){
        vec<dim, T> res;
        for (size_t i = dim; i--; res._data[i] -= this->_data[i] + rhs._data[i]);
        return res;
    }

    vec<dim, T>& operator-= (const vec<dim, T>& rhs){
        for (size_t i = dim; i--; this->_data[i] -= rhs._data[i]);
        return *this;
    }

    template <typename U>
    vec<dim, T> operator*(const U& rhs){
        vec<dim, T> res;
        for (size_t i = dim; i--; res._data[i] = this->data[i]*rhs);
        return res;
    }

    template<typename U>
    vec<dim, T>& operator *=(const U& rhs){
        for (size_t i = dim; i--; this->_data[i] *= rhs);
        return *this;
    }

    template <typename U>
    vec<dim, T> operator/(const U& rhs){
        vec<dim, T> res;
        for (size_t i = dim; i--; res._data[i] = this->_data[i]/rhs);
        return res;
    }

    template<typename U>
    vec<dim, T>& operator /=(const U& rhs){
        for (size_t i = dim; i--; this->_data[i] /= rhs);
        return *this;
    }

    T _data[dim];
};



// tempalate specialization for 2-dimentional vector
template <typename T>
struct vec<2, T>{
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}
    template <typename U> vec<2, T>(const vec<2,U> &v);

    T& operator[](const size_t i) {
        return i <= 0 ? x : y;
    }

    const T& operator[](const size_t i) const{
        return i <= 0 ? x : y;
    }

    T operator* (const vec<2, T>& rhs){
        T ret;
        ret = this->x * rhs.x + this->y * rhs.y;
        return ret;
    }

    vec<2, T> operator+ (const vec<2, T>& rhs){
        vec<2, T> res;
        res.x = this->x + rhs.x;
        res.y = this->y + rhs.y;
        return res;
    }

    vec<2, T>& operator+= (const vec<2, T>& rhs){
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vec<2, T> operator- (const vec<2, T>& rhs){
        vec<2, T> res;
        res.x = this->x - rhs.x;
        res.y = this->y - rhs.y;
        return res;
    }

    vec<2, T>& operator-= (const vec<2, T>& rhs){
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template <typename U>
    vec<2, T> operator*(const U& rhs){
        vec<2, T> res;
        res.x = this->x * rhs;
        res.y = this->y * rhs;
        return res;
    }

    template<typename U>
    vec<2, T>& operator *=(const U& rhs){
        x *= rhs;
        y *= rhs;
        return *this;
    }

    template <typename U>
    vec<2, T> operator/(const U& rhs){
        vec<2, T> res;
        res.x = this->x / rhs;
        res.y = this->y / rhs;
        return res;
    }

    template<typename U>
    vec<2, T>& operator /=(const U& rhs){
        x /= rhs;
        y /= rhs;
        return *this;
    }

    T x, y;
};


// template specialization for 3-dimentional vector
template <typename T>
struct vec<3, T>{
    vec() : x(T()), y(T()), z(T()) {}
    vec (T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    template <typename U> vec<3, T> (const vec<3, U> &v);

    T& operator[](const size_t i){
        return i <= 0 ? x : (i == 1 ? y : z);
    }

    const T& operator[](const size_t i) const{
        return i <= 0 ? x : (i == 1 ? y : z);
    }

    T operator* (const vec<3, T>& rhs){
        T ret;
        ret = this->x*rhs.x + this->y*rhs.y + this->z*rhs.z;
        return ret;
    }

    vec<3, T> operator+ (const vec<3, T>& rhs){
        vec<3, T> res;
        res.x = this->x + rhs.x;
        res.y = this->y + rhs.y;
        res.z = this->z + rhs.z;
        return res;
    }

    vec<3, T>& operator+= (const vec<3, T>& rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    vec<3, T> operator- (const vec<3, T>& rhs){
        vec<3, T> res;
        res.x = this->x - rhs.x;
        res.y = this->y - rhs.y;
        res.z = this->z - rhs.z;
        return res;
    }

    vec<3, T>& operator-= (const vec<3, T>& rhs){
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    template <typename U>
    vec<3, T> operator*(const U& rhs){
        vec<3, T> res;
        res.x = this->x * rhs;
        res.y = this->y * rhs;
        res.z = this->z * rhs;
        return res;
    }

    template<typename U>
    vec<3, T>& operator *=(const U& rhs){
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    template <typename U>
    vec<3, T> operator/(const U& rhs){
        vec<3, T> res;
        res.x = this->x / rhs;
        res.y = this->y / rhs;
        res.z = this->z / rhs;
        return res;
    }

    template<typename U>
    vec<3, T>& operator /=(const U& rhs){
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    double length() {
       return std::sqrt(x*x + y*y + z*z);
    }

    vec <3, T>& normalize (T l = 1){
       *this *= l/sqrt(x*x + y*y + z*z);
       return *this;
    }

    T x, y, z;
};



        // расширение/уменьшение вектора
template <size_t len, size_t dim, typename T>
vec<len, T> embed(const vec<dim, T> &v, T fill = 1){
    vec<len, T> ret;
    for (size_t i = len; i--; ret[i] = (i < dim ? v[i] : fill));
    return ret;
}

template <size_t len, size_t dim, typename T>
vec<len, T> proj (const vec<dim, T> &v){
    vec<len, T> ret;
    for (size_t i = len; i--; ret[i] = v[i]/v[3]); // УБЕРИ!!!!!!!
    return ret;
}

template <typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2){
    return vec<3, T> (v1.y * v2.z - v1.z * v2.y,
                      v1.z * v2.x - v1.x * v2.z,
                      v1.x * v2.y - v1.y * v2.x);
}

template <size_t dim, typename T>
std::ostream& operator << (std::ostream& out, vec<dim, T>& v){
    for (size_t i = 0; i < dim; i++)
        out << v[i] << " ";
    return out;
}

typedef vec<2, double> Vec2d;
typedef vec<2, int>   Vec2i;
typedef vec<3, double> Vec3d;
typedef vec<3, int>   Vec3i;
typedef vec<4, double> Vec4d;

#endif // GEOMETRY_H
