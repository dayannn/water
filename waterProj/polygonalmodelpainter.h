#ifndef POLYGONALMODELPAINTER_H
#define POLYGONALMODELPAINTER_H

#include "basepainter.h"
#include "camera.h"
#include "baseobject.h"
#include "basecanvas.h"
#include "model.h"
#include "matrix.h"
#include <vector>

using std::vector;

class PolygonalModelPainter : public BasePainter
{
public:
    PolygonalModelPainter();
    ~PolygonalModelPainter();

    virtual void draw(BaseCanvas *canvas, BaseObject *object, Camera *camera) override;
};


#endif // POLYGONALMODELPAINTER_H
