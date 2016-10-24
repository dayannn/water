#ifndef MATRIX_H
#define MATRIX_H

#include "geometry.h"

// !!!!!!!!!!!!!
// MAKE MATRICES INSIDE METHODS STATIC????

// TODO: release *= operator

/*class Matrix {
    std::vector<std::vector<double>> m;
    int rows, cols;
public:
    Matrix(int r = 4, int c = 4);
    Matrix(Vec3d v);
    int nrows();
    int ncols();

    static Matrix identity(int dimensions);
    std::vector<double>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix& operator*= (const Matrix& a);
    void multLeft (const Matrix &a);
    Matrix transpose();
    Matrix inverse();

    static Matrix rotateX(double angle);
    static Matrix rotateY(double angle);
    static Matrix rotateZ(double angle);
    static Matrix move(double dx, double dy, double dz);
    static Matrix scale(double angle);

    friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

const int depth  = 255;
Matrix viewport(int x, int y, int w, int h);*/

/* ############################################ */
/*            NEW MATRIX CLASS                  */
/* ############################################ */

template <size_t rows, size_t cols, typename T> class matr;

template<size_t dim, typename T> struct dt{
    static T det(const matr<dim, dim, T>& src){
        T ret = 0;
        for (size_t i = dim; i--; ret += src[0][i] * src.cofactor(0, i));
        return ret;
    }
};

template<typename T> struct dt<1,T>{
    static T det(const matr<1, 1, T>& src){
        return src[0][0];
    }
};

template <size_t rows, size_t cols, typename T>
class matr {
private:
    vec<cols, T> _rows[rows];

public:
    matr() {}

    vec<cols, T>& operator[] (const size_t idx){
        // exc idx < rows
        return _rows[idx];
    }

    const vec<cols, T>& operator[] (const size_t idx) const{
        // exc
        return _rows[idx];
    }

    vec<rows, T> col (const size_t idx) const{
        // exc
        vec<rows, T> ret;
        for (size_t i = rows; i--;ret[i] = _rows[i][idx])
        return ret;
    }

    void set_col(size_t idx, vec<rows, T> v){
        // exc
        for (size_t i = cols; i--; _rows[i][idx] = v[i]);
    }

            // единичная матрица
    static matr<rows, cols, T> identity() {
        matr<rows, cols, T> ret;
        for (size_t i = rows; i--; )
            for (size_t j = cols; j--; ret[i][j] = (i==j));
        return ret;
    }

    T det() const{
        return dt<cols, T>::det(*this);
    }

    matr<rows-1, cols-1, T> get_minor(size_t row, size_t col) const{
        matr<rows-1, cols-1, T> ret;
        for (size_t i = rows-1; i--; )
            for(size_t j = cols-1; j--; )
                ret[i][j] = _rows[i < row ? i : i+1][j < col ? j : j+1];
        return ret;
    }

    T cofactor(size_t row, size_t col) const{
        return get_minor(row, col).det()*((row + col)%2 ? -1 : 1);
    }

    matr<rows, cols, T> adjugate() const{
        matr<rows, cols, T> ret;
        for (size_t i = rows; i--; )
            for (size_t j = cols; j--; )
                ret[i][j] = cofactor(i, j);
        return ret;
    }

    matr<rows, cols, T> invert_transpose() {
        matr<rows, cols, T> ret = adjugate();
        T tmp = ret[0]*_rows[0];
        return ret/tmp;
    }

    vec<rows, T> operator* (const vec<cols, T>& rhs){
        vec<rows, T> ret;
        for (size_t i = rows; i--; )
            ret[i] = (*this)[i] * rhs;
        return ret;
    }

    template <size_t cols2>
    matr<rows, cols2, T> operator* (const matr<cols, cols2, T>& rhs){
        matr<rows, cols2, T> ret;
        for (size_t i = rows; i--; )
            for (size_t j = cols2; j--; ret[i][j] = (*this)[i] * rhs.col(j));
        return ret;
    }

    matr<rows, rows, T> operator* (const matr<rows, rows, T>& rhs)
    {
        matr<rows, rows, T> ret;
        for (size_t i = rows; i--; )
            for (size_t j = rows; j--; ret[i][j] = (*this)[i] * rhs.col(j));
        return ret;
    }

    matr<cols, rows, T> operator/ (const T& rhs){
        matr<cols, rows, T> ret;
        for (size_t i = rows; i--; ret[i] = (*this)[i] / rhs);
        return ret;
    }

    matr<cols, rows, T>& operator /= (const T& rhs){
        for (size_t i = rows; i--; (*this)[i] /= rhs);
        return *this;
    }
};


template <typename T>
class matr<4, 4, T> {
private:
    vec<4, T> _rows[4];

public:
    matr() {}

    vec<4, T>& operator[] (const size_t idx){
        // exc idx < rows
        return _rows[idx];
    }

    const vec<4, T>& operator[] (const size_t idx) const{
        // exc
        return _rows[idx];
    }

    vec<4, T> col (const size_t idx) const{
        // exc
        vec<4, T> ret;
        for (size_t i = 4; i--;ret[i] = _rows[i][idx])
        return ret;
    }

    void set_col(size_t idx, vec<4, T> v){
        // exc
        for (size_t i = 4; i--; _rows[i][idx] = v[i]);
    }

            // единичная матрица
    static matr<4, 4, T> identity() {
        matr<4, 4, T> ret;
        for (size_t i = 4; i--; )
            for (size_t j = 4; j--; ret[i][j] = (i==j));
        return ret;
    }

    T det() const{
        return dt<4, T>::det(*this);
    }

    matr<3, 3, T> get_minor(size_t row, size_t col) const{
        matr<3, 3, T> ret;
        for (size_t i = 3; i--; )
            for(size_t j = 3; j--; )
                ret[i][j] = _rows[i < row ? i : i+1][j < col ? j : j+1];
        return ret;
    }

    T cofactor(size_t row, size_t col) const{
        return get_minor(row, col).det()*((row + col)%2 ? -1 : 1);
    }

    matr<4, 4, T> adjugate() const{
        matr<4, 4, T> ret;
        for (size_t i = 4; i--; )
            for (size_t j = 4; j--; )
                ret[i][j] = cofactor(i, j);
        return ret;
    }

    matr<4, 4, T> invert_transpose() {
        matr<4, 4, T> ret = adjugate();
        T tmp = ret[0]*_rows[0];
        return ret/tmp;
    }

    vec<4, T> operator* (const vec<4, T>& rhs){
        vec<4, T> ret;
        for (size_t i = 4; i--; )
            ret[i] = (*this)[i] * rhs;
        return ret;
    }

    matr<4, 4, T> operator* (const matr<4, 4, T>& rhs)
    {
        matr<4, 4, T> ret;
        for (int i = 0; i < 4; i++) {
            for (int  j = 0; j < 4; j++) {
                ret[i][j] = 0.f;
                for (int k = 0; k < 4; k++) {
                    ret[i][j] += (*this)[i][k] * rhs[k][j];
                }
            }
        }
        return ret;
    }

    matr<4, 4, T>& operator*= (const matr<4, 4, T>& rhs)
    {
        matr<4, 4, T> temp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[i][j] = (*this)[i][j];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                (*this)[i][j] = 0.f;
                for (int k = 0; k < 4; k++) {
                    (*this)[i][j] += temp[i][k] * rhs[k][j];
                }
            }
        }
        return *this;
    }

    void multLeft (const matr<4, 4, T>& a)
    {
        matr<4, 4, T> temp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[i][j] = (*this)[i][j];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                (*this)[i][j] = 0.f;
                for (int k = 0; k < 4; k++) {
                    (*this)[i][j] += a[i][k] * temp[k][j];
                }
            }
        }
    }

    matr<4, 4, T> operator/ (const T& rhs){
        matr<4, 4, T> ret;
        for (size_t i = 4; i--; ret[i] = (*this)[i] / rhs);
        return ret;
    }

    matr<4, 4, T>& operator /= (const T& rhs){
        for (size_t i = 4; i--; (*this)[i] /= rhs);
        return *this;
    }

    static matr<4, 4, T> rotateX(double angle){
        matr<4, 4, T> m;

        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = cos(angle);
        m[1][2] = -sin(angle);
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = sin(angle);
        m[2][2] = cos(angle);
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return m;
    }

    static matr<4, 4, T> rotateY(double angle)
    {
        matr<4, 4, T> m;

        m[0][0] = cos(angle);
        m[0][1] = 0;
        m[0][2] = sin(angle);
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = -sin(angle);
        m[2][1] = 0;
        m[2][2] = cos(angle);
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return m;
    }

    static matr<4, 4, T>rotateZ(double angle)
    {
        matr<4, 4, T> m;

        m[0][0] = cos(angle);
        m[0][1] = -sin(angle);
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = sin(angle);
        m[1][1] = cos(angle);
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return m;
    }

    static matr<4, 4, T> move(double dx, double dy, double dz)
    {
        matr<4, 4, T> m;

        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = dx;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = dy;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = dz;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return m;
    }

    static matr<4, 4, T> scale(double scale)
    {
        matr<4, 4, T> m;

        m[0][0] = scale;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = scale;
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = scale;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return m;
    }

    static matr<4, 4, T> viewport(int x, int y, int w, int h) {
        matr<4, 4, T> m = matr<4, 4, T>::identity();

        int depth = 255;

        m[0][3] = x+w/2.f;
        m[1][3] = y+h/2.f;
        m[2][3] = depth/2.f;

        m[0][0] = w/2.f;
        m[1][1] = h/2.f;
        m[2][2] = depth/2.f;
        return m;
    }
};

typedef matr<4, 4, double> Matrix;

#endif // MATRIX_H
