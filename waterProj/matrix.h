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

#endif // MATRIX_H
