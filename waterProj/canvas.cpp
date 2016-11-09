#include "canvas.h"
#include <QPen>

Canvas::Canvas(const CanvasInfo* canvasInfo){

    info.paintWidget = canvasInfo->paintWidget;
    pen = new QPen();
    pen->setWidth(1.0);
    pen->setColor(QColor(147, 29, 52));
}

Canvas::~Canvas()
{
    clear();
    info.paintWidget = nullptr;
    delete pen;
}

void Canvas::clear()
{
    if (!this->info.paintWidget)
    {
        throw EmptyCanvasException();
    }
    this->info.paintWidget->clear();
}

void Canvas::drawEdge(double x1, double y1, double x2, double y2, QColor& modelColor)
{
    if (!info.paintWidget)
    {
        throw EmptyCanvasException();
    }
    info.paintWidget->drawLine(x1, y1, x2, y2, modelColor);
}

void Canvas::drawPoint(double x, double y, QColor &modelColor)
{
    if (!info.paintWidget)
    {
        throw EmptyCanvasException();
    }
    info.paintWidget->drawLine(x, y, x, y, modelColor);
}

void Canvas::update()
{
    // repaint ?
    info.paintWidget->update();
}

int Canvas::width()
{
    return info.paintWidget->width();
}

int Canvas::height()
{
    return info.paintWidget->height();
}

void Canvas::fillTriangle(Vec3d* verts, Vec3d *real_verts, Vec3d* norms, Vec3d& light, Vec3d &camera, QColor &modelColor)
{
    info.paintWidget->fillTriangle(verts, real_verts, norms, light, camera, modelColor);
}

void Canvas::prepareZBuf()
{
    info.paintWidget->prepareZBuf();
}
