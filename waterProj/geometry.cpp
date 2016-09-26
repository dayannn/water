#include "geometry.h"
#include "matrix.h"

template <> Vec3<double>::Vec3(Matrix m) : x(m[0][0]/m[3][0]), y(m[1][0]/m[3][0]), z(m[2][0]/m[3][0]) {}

template <> template <> Vec3<int>::Vec3<>(const Vec3<double> &v)
    : x(static_cast<int>(v.x+.5)), y(static_cast<int>(v.y+.5)), z(static_cast<int>(v.z+.5)) {}

template <> template <> Vec3<double>::Vec3<>(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {}


