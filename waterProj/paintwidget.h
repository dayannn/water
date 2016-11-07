#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include "model.h"
#include "geometry.h"
#include <QLineF>
#include <QPen>
#include <vector>

using std::vector;

struct pLine{
    QLineF line;
    QPen pen;
};

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    ~PaintWidget();

    void drawLine(double x1, double y1, double x2, double y2, QColor &modelColor);
    void fillTriangle(Vec3i &v0, Vec3i &v1, Vec3i &v2, double ity0, double ity1, double ity2, QColor &modelColor);
    void fillTexturedTriangle(Model &mdl, Vec3i &v0, Vec3i &v1, Vec3i &v2, Vec2i& uv0, Vec2i& uv1, Vec2i& uv2, QImage& texture, double intensity);

    void clear();
    void setImg();
    void prepareZBuf();

protected:
    void paintEvent (QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    /*void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);*/

private:
    QImage *img;
    QColor bgColor;

    int **zbuffer;
    int zbufHeight;

    vector<pLine> linesVect;

signals:

public slots:
};

Vec3d barycentric (Vec2d A, Vec2d B, Vec2d C, Vec2d P);

#endif // PAINTWIDGET_H
