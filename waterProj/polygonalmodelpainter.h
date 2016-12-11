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
    virtual void setLightPoint(Vec3d light) override;
    virtual void setAmbLight(QColor *clr) override;
    virtual void setDiffLight(QColor* clr) override;
    virtual void setSpecLight(QColor* clr) override;

private:
    light_source _light;
};


#endif // POLYGONALMODELPAINTER_H
