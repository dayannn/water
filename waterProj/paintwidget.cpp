#include "paintwidget.h"

#include <QPainter>


void free_matrix_rows (double **p, int strs)
{
    for (int i = 0; i < strs; i++)
        delete[] p[i];
    delete[] p;
}

double** allocate_matrix_rows (int strs, int cols)
{
    double **p = new double*[strs];
    if (!p)
        return NULL;
    for (int i = 0; i < strs; i++)
    {
        p[i] = new double [cols];
        if (!p[i])
        {
            free_matrix_rows (p, strs);
            return NULL;
        }
    }
    return p;
}

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    img = new QImage(0, 0, QImage::Format_ARGB32);

    zbuffer = nullptr;
    zbufHeight = 0;

    bgColor = QColor(90, 90, 90);
}

PaintWidget::~PaintWidget()
{
    delete img;
    free_matrix_rows(zbuffer, zbufHeight);
}

void PaintWidget::drawLine(double x1, double y1, double x2, double y2, QColor& modelColor)
{   
    int width = this->width();
    int height = this->height();

    //unsigned clr = qRgb(modelColor.red(), modelColor.green(), modelColor.blue());
    unsigned clr = qRgb(0, 0, 0);

    if (x2 == x1 && y2 == y1)
    {
        if (x1 > 0 && x1 < width && y1 > 0 && y1 < height)
            img->setPixel(x1, height - y1, clr);
        return;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    int sx;
    int sy;

    if (dx == 0)
        sx = 0;
    else
        sx = (dx > 0 ? 1 : -1);
    if (dy == 0)
        sy = 0;
    else
        sy = (dy > 0 ? 1 : -1);

    dx = fabs(dx);
    dy = fabs(dy);
    bool ob;
    if (dx > dy)
        ob = 0;
    else
    {
        ob = 1;
        int t = dx;
        dx = dy;
        dy = t;
    }
    double m = 1.*dy/dx;
    double e = m - 0.5;

    int x = x1;
    int y = y1;
    for (int i = 0; i < dx+1; i++)
    {
        if (x > 0 && x < width && y > 0 && y < height)
            img->setPixel(x, height - y, clr);
        if (e >= 0)
        {
            if (ob)
                x += sx;
            else
                y += sy;
            e -= 1;
        }
        if (ob)
            y += sy;
        else
            x += sx;
        e += m;
    }
}

Vec3d barycentric(Vec2d A, Vec2d B, Vec2d C, Vec2d P) {
    Vec3d s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3d u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3d(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3d(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void PaintWidget::fillTriangle(Vec3d* verts, Vec3d *real_verts, Vec3d* norms, Vec3d& light, Vec3d& camera, QColor &modelColor)
{
    int width = this->width();
    int height = this->height();

    Vec3i v0 = verts[0];
    Vec3i v1 = verts[1];
    Vec3i v2 = verts[2];

    if (v0.y == v1.y && v0.y == v2.y)
        return;
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
        std::swap(verts[0], verts[1]);
        std::swap(norms[0], norms[1]);
        std::swap(real_verts[0], real_verts[1]);
    }

    if (v0.y > v2.y)
    {
        std::swap(v0, v2);
        std::swap(verts[0], verts[2]);
        std::swap(norms[0], norms[2]);
        std::swap(real_verts[0], real_verts[2]);
    }

    if (v1.y > v2.y)
    {
        std::swap(v1, v2);
        std::swap(verts[1], verts[2]);
        std::swap(norms[1], norms[2]);
        std::swap(real_verts[1], real_verts[2]);
    }

    int total_height = v2.y - v0.y;

    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > v1.y - v0.y || v1.y == v0.y;
        int segment_height = second_half ? v2.y - v1.y : v1.y - v0.y;
        double alpha = (double)i/total_height;
        double beta  = (double)(i-(second_half ? v1.y - v0.y : 0))/segment_height;

        Vec3d dAn = norms[0] + (norms[2]-norms[0])*alpha;
        Vec3d dBn = second_half ? norms[1] + (norms[2]-norms[1])*beta : norms[0] + (norms[1]-norms[0])*beta;

        Vec3d dA = real_verts[0] + (real_verts[2]-real_verts[0])*alpha;
        Vec3d dB = second_half ? real_verts[1] + (real_verts[2]-real_verts[1])*beta : real_verts[0] + (real_verts[1]-real_verts[0])*beta;

        Vec3d ddA = verts[0] + (verts[2] - verts[0])*alpha;
        Vec3d ddB = second_half ? verts[1] + (verts[2]-verts[1])*beta : verts[0] + (verts[1]-verts[0])*beta;

        Vec3i A = v0 + Vec3d(v2-v0)*alpha;
        Vec3i B = second_half ? v1 + Vec3d(v2-v1)*beta : v0 + Vec3d(v1-v0)*beta;

        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(dAn, dBn);
            std::swap(dA, dB);
        }
        for (int j = A.x; j <= B.x; j++)
        {
            double phi = B.x == A.x ? 1.0 : (double)(j - A.x)/double(B.x - A.x);
            Vec3i P = Vec3d(A) + Vec3d(B-A)*phi;
            Vec3d dPn = dAn + (dBn-dAn)*phi;
            Vec3d dP = dA + (dB-dA)*phi;
            Vec3d ddP = ddA + (ddB-ddA)*phi;
            Vec3d light_dir = (dP - light).normalize();

            //Vec3d bar = barycentric(proj<2>(real_verts[0]), proj<2>(real_verts[1]), proj<2>(real_verts[2]), proj<2>(dP));

            Vec3d r = (dPn*(dPn*light_dir*2.f) - light_dir).normalize();
            Vec3d v = (dP - camera).normalize();
            double reflection = pow(std::max(0.0, r*v), 50);

            double ity = std::min(std::max(0.2, dPn*light_dir), 1.0);
            // с учётом уменьшения интенсивности с расстоянием
            //double ity = std::max(0.0, dPn*light_dir/pow((dP - light).length(),2)*100);
            //double ity = 0;

            if (P.x > 0 && P.x < width && P.y > 0 && P.y < height)
            {
                if (ddP.z - zbuffer[P.y][P.x] > 10e-2)
                {
                    zbuffer[P.y][P.x] = ddP.z;
                    img->setPixel(P.x, height - P.y, qRgb(std::min(modelColor.red()*ity + 255*reflection, 255.0),
                                                          std::min(modelColor.green()*ity + 255*reflection, 255.0),
                                                          std::min(modelColor.blue()*ity + 255*reflection, 255.0)));
                }
            }
        }
    }
}

void PaintWidget::clear()
{
    img->fill(bgColor);
}

void PaintWidget::setImg()
{
    delete img;
    img = new QImage(this->width(), this->height(), QImage::Format_ARGB32);

    free_matrix_rows(zbuffer, zbufHeight);
    zbuffer = allocate_matrix_rows(height(), width());
    zbufHeight = height();

}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, *img);
}

void PaintWidget::resizeEvent(QResizeEvent *)
{
    delete img;
    img = new QImage(this->width(), this->height(), QImage::Format_ARGB32);

    free_matrix_rows(zbuffer, zbufHeight);
    zbuffer = allocate_matrix_rows(height(), width());
    zbufHeight = height();

}

void PaintWidget::prepareZBuf()
{
    int min = std::numeric_limits<double>::min();
    int h = height();
    int w = width();
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            zbuffer[i][j] = min;
}


