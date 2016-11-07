#ifndef BASECANVAS_H
#define BASECANVAS_H

#include "paintwidget.h"


struct CanvasInfo
{
    PaintWidget* paintWidget;
};

class BaseCanvas
{
public:
    virtual ~BaseCanvas() {}

    virtual int width() = 0;
    virtual int height() = 0;
    virtual void update() = 0;
    virtual void clear() = 0;
    virtual void drawPoint(double, double, QColor&) = 0;
    virtual void drawEdge(double, double, double, double, QColor&) = 0;
    virtual void fillTriangle(Vec3i &v0, Vec3i &v1, Vec3i &v2, double ity0, double ity1, double ity2, QColor& modelColor) = 0;
    virtual void prepareZBuf() = 0;
};

#endif // BASECANVAS_H
