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
};


#endif // BASEPAINTER_H
