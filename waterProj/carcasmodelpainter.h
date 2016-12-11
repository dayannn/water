#ifndef CARCASMODELPAINTER_H
#define CARCASMODELPAINTER_H

#include "basepainter.h"
#include "camera.h"
#include "baseobject.h"
#include "basecanvas.h"
#include "model.h"
#include <vector>

using std::vector;


class CarcasModelPainter : public BasePainter
{
public:
    CarcasModelPainter();
    ~CarcasModelPainter();

    virtual void draw(BaseCanvas *, BaseObject *, Camera *) override;
    virtual void setLightPoint(Vec3d light) override;
    virtual void setAmbLight(QColor *clr) override;
    virtual void setDiffLight(QColor* clr) override;
    virtual void setSpecLight(QColor* clr) override;
};

#endif // CARCASMODELPAINTER_H
