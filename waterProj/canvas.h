#ifndef CANVAS_H
#define CANVAS_H

#include "basecanvas.h"
#include "baseexception.h"

class Canvas : public BaseCanvas
{
private:
    CanvasInfo info;
    QPen *pen;

public:
    Canvas(const CanvasInfo*);
    ~Canvas();

    virtual int width() override;
    virtual int height() override;
    virtual void update() override;
    virtual void clear() override;
    virtual void drawEdge(double, double, double, double, QColor &modelColor) override;
    virtual void drawPoint(double, double, QColor &modelColor) override;
    virtual void fillTriangle(Vec3d* verts, Vec3d *real_verts, Vec3d* norms, Vec3d& light, Vec3d &camera, LightKoefs* koefs) override;
    virtual void prepareZBuf() override;
};

class EmptyCanvasException : public BaseException
{
public:
    virtual const char* what()
    {
        return "Canvas instance is empty!";
    }
};


#endif // CANVAS_H
