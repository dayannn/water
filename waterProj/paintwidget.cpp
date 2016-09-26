#include "paintwidget.h"

#include <QPainter>


void free_matrix_rows (int **p, int strs)
{
    for (int i = 0; i < strs; i++)
        delete[] p[i];
    delete[] p;
}

int** allocate_matrix_rows (int strs, int cols)
{
    int **p = new int*[strs];
    if (!p)
        return NULL;
    for (int i = 0; i < strs; i++)
    {
        p[i] = new int [cols];
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

    bgColor = QColor(40, 40, 40);
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

void PaintWidget::fillTriangle(Vec3i &v0, Vec3i &v1, Vec3i &v2, double ity0, double ity1, double ity2, QColor &modelColor)
{
    if (ity0 < 0)
        ity0 = 0;
    if (ity1 < 0)
        ity1 = 0;
    if (ity2 < 0)
        ity2 = 0;

    int width = this->width();
    int height = this->height();

    if (v0.y == v1.y && v0.y == v2.y)
        return;
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
        std::swap(ity0, ity1);
    }
    if (v0.y > v2.y)
    {
        std::swap(v0, v2);
        std::swap(ity0, ity2);
    }
    if (v1.y > v2.y)
    {
        std::swap(v1, v2);
        std::swap(ity1, ity2);
    }

    int total_height = v2.y - v0.y;

    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > v1.y - v0.y || v1.y == v0.y;
        int segment_height = second_half ? v2.y - v1.y : v1.y - v0.y;
        double alpha = (double)i/total_height;
        double beta  = (double)(i-(second_half ? v1.y - v0.y : 0))/segment_height;

        Vec3i A = v0 + Vec3d(v2-v0)*alpha;
        Vec3i B = second_half ? v1 + Vec3d(v2-v1)*beta : v0 + Vec3d(v1-v0)*beta;

        double ityA = ity0 + (ity2 - ity0) * alpha;
        double ityB = second_half ? ity1 + (ity2-ity1)*beta : ity0 + (ity1-ity0) * beta;

        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(ityA, ityB);
        }
        for (int j = A.x; j <= B.x; j++)
        {
            double phi = B.x == A.x ? 1.0 : (double)(j - A.x)/double(B.x - A.x);
            Vec3i P = Vec3d(A) + Vec3d(B-A)*phi;
            double ityP = ityA + (ityB-ityA)*phi;

            if (P.x > 0 && P.x < width && P.y > 0 && P.y < height)
            {
                if (zbuffer[P.y][P.x] < P.z)
                {
                    zbuffer[P.y][P.x] = P.z;
                    img->setPixel(P.x, height - P.y, qRgb(modelColor.red()*ityP, modelColor.green()*ityP, modelColor.blue()*ityP));
                }
            }
        }
    }
}

void PaintWidget::fillTexturedTriangle(Model& mdl, Vec3i &v0, Vec3i &v1, Vec3i &v2, Vec2i &uv0, Vec2i &uv1, Vec2i &uv2, QImage &texture, double intensity)
{
   // QPainter p(img);
   // p.drawImage(0, 0, texture);

    if (v0.y == v1.y && v0.y == v2.y)
        return;
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
        std::swap(uv0, uv1);
    }
    if (v0.y > v2.y)
    {
        std::swap(v0, v2);
        std::swap(uv0, uv2);
    }
    if (v1.y > v2.y)
    {
        std::swap(v1, v2);
        std::swap(uv1, uv2);
    }

    int total_height = v2.y - v0.y;
    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > v1.y-v0.y || v1.y == v0.y;
        int segment_height = second_half ? v2.y-v1.y : v1.y-v0.y;
        double alpha = (double)i/total_height;
        double beta  = (double)(i-(second_half ? v1.y-v0.y : 0))/segment_height;

        Vec3i A = v0 + Vec3d(v2-v0)*alpha;
        Vec3i B = second_half ? v1 + Vec3d(v2-v1 )*beta : v0 + Vec3d(v1-v0)*beta;
        Vec2i uvA = uv0 +(uv2-uv0)*alpha;
        Vec2i uvB = second_half ? uv1 + (uv2-uv1)*beta : uv0 + (uv1-uv0)*beta;

        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(uvA, uvB);
        }
        for (int j = A.x; j <= B.x; j++)
        {
            double phi = B.x == A.x ? 1. : (double)(j-A.x)/(double)(B.x-A.x);
            Vec3i P = Vec3d(A) + Vec3d(B-A)*phi;
            Vec2i uvP = uvA + (uvB-uvA)*phi;
            if (P.x > 0 && P.x < width() && P.y > 0 && P.y < height())
                if (zbuffer[P.y][P.x] < P.z)
                {
                    zbuffer[P.y][P.x] = P.z;
                    QColor color = mdl.diffuse(uvP);
                    img->setPixel(P.x, P.y, qRgb(color.red()*intensity, color.green()*intensity, color.blue()*intensity));
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
    int min = std::numeric_limits<int>::min();
    int h = height();
    int w = width();
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            zbuffer[i][j] = min;
}
