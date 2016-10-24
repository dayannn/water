#include "matrix.h"


/*Matrix::Matrix(Vec3d v) : m(std::vector<std::vector<double> >(4, std::vector<double>(1, 1.f))), rows(4), cols(1) {
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
}

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<double> >(r, std::vector<double>(c, 0.f))), rows(r), cols(c) { }

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<double>& Matrix::operator[](const int i) {

    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {

    Matrix result(rows, a.cols);
    for (int i = 0; i < rows; i++) {
        for (int  j = 0; j < a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                result.m[i][j] += m[i][k] * a.m[k][j];
            }
        }
    }
    return result;
}

Matrix& Matrix::operator*= (const Matrix& a){
    Matrix temp(4, 4);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = this->m[i][j];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            this->m[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                this->m[i][j] += temp.m[i][k] * a.m[k][j];
            }
        }
    }
    return *this;
}

void Matrix::multLeft (const Matrix &a)
{
    Matrix temp(4, 4);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = this->m[i][j];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            this->m[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                this->m[i][j] += a.m[i][k] * temp.m[k][j];
            }
        }
    }
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {

    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols*2);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[i][j] = m[i][j];
    for(int i=0; i<rows; i++)
        result[i][i+cols] = 1;
    // first pass
    for (int i=0; i<rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result[i][j] /= result[i][i];
        for (int k=i+1; k<rows; k++) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=rows-1; j--)
        result[rows-1][j] /= result[rows-1][rows-1];
    // second pass
    for (int i=rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            truncate[i][j] = result[i][j+cols];
    return truncate;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.nrows(); i++)  {
        for (int j=0; j<m.ncols(); j++) {
            s << m[i][j];
            if (j<m.ncols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}

Matrix Matrix::rotateX(double angle)
{
    Matrix m;

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

Matrix Matrix::rotateY(double angle)
{
    Matrix m;

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

Matrix Matrix::rotateZ(double angle)
{
    Matrix m;

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

Matrix Matrix::move(double dx, double dy, double dz)
{
    Matrix m;

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

Matrix Matrix::scale(double scale)
{
    Matrix m;

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

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}*/


/* ############################################ */
/*            NEW MATRIX CLASS                  */
/* ############################################ */









