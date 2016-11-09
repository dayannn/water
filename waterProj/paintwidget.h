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
    void fillTriangle(Vec3d* verts, Vec3d *real_verts, Vec3d* norms, Vec3d& light, Vec3d &camera, QColor &modelColor);

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
