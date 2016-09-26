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

void Canvas::fillTriangle(Vec3i &v0, Vec3i &v1, Vec3i &v2, double ity0, double ity1, double ity2, QColor& modelColor)
{
    info.paintWidget->fillTriangle(v0, v1, v2, ity0, ity1, ity2, modelColor);
}

void Canvas::fillTexturedTriangle(Model &mdl, Vec3i &v0, Vec3i &v1, Vec3i &v2, Vec2i &uv0, Vec2i &uv1, Vec2i &uv2, QImage &img, double intensity)
{
    info.paintWidget->fillTexturedTriangle(mdl, v0, v1, v2, uv0, uv1, uv2, img, intensity);
}

void Canvas::prepareZBuf()
{
    info.paintWidget->prepareZBuf();
}
