#ifndef BASEPAINTER_H
#define BASEPAINTER_H

#include "basecanvas.h"
#include "model.h"
#include "camera.h"

class BasePainter
{
public:
    virtual ~BasePainter() {}
    virtual void draw(BaseCanvas*, BaseObject*, Camera*) = 0;
    virtual void setLightPoint(Vec3d light) = 0;
    virtual void setAmbLight(QColor *clr) = 0;
    virtual void setDiffLight(QColor* clr) = 0;
    virtual void setSpecLight(QColor* clr) = 0;
};


#endif // BASEPAINTER_H
