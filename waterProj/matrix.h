#ifndef MATRIX_H
#define MATRIX_H

#include "geometry.h"


// TODO: release *= operator

class Matrix {
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
Matrix viewport(int x, int y, int w, int h);

/* ############################################ */
/*            NEW MATRIX CLASS                  */
/* ############################################ */

/*template <size_t rows, size_t cols, typename T> class matr;

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

    vec<cols, T> col (const size_t idx) const{
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
            for(size_t j = cols; j--; )
                ret[i][j] = _rows[i < row ? i : i+1][j < cols ? j : j+1];
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
matr<4, 4, T> matr<4, 4, T>::operator* (const matr<4, 4, T>& rhs)
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

typedef matr<4, 4, double> Matrix;*/

#endif // MATRIX_H
