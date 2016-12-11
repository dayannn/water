#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include "model.h"
#include "geometry.h"
#include <QPen>
#include <vector>

struct light_source{
    Vec3d point;
    int amb_r;
    int amb_g;
    int amb_b;
    int diff_r;
    int diff_g;
    int diff_b;
    int spec_r;
    int spec_g;
    int spec_b;
};

using std::vector;

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    ~PaintWidget();

    void drawLine(double x1, double y1, double x2, double y2, QColor &modelColor);
    void fillTriangle(Vec3d* verts, Vec3d *real_verts, Vec3d* norms, light_source* light, Vec3d &camera, LightKoefs *koefs);

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

    double **zbuffer;
    int zbufHeight;

signals:

public slots:
};

#endif // PAINTWIDGET_H
