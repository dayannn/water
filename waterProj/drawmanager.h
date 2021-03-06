#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H


#include "basepainter.h"
#include "basecanvas.h"

#include "compositeobject.h"
#include "camera.h"
#include "basescene.h"

#include <vector>

class DrawManager
{
private:
    BaseCanvas*   canvas;
    BasePainter*  painter;

public:
    DrawManager(const CanvasInfo*);
    ~DrawManager();

    void drawObject (BaseObject*, Camera *);
    void drawObject (CompositeObject*, Camera*);
    void drawScene  (BaseScene*, vector<Camera*>::iterator&);

    void changeModelType (eModelType type);
    void setLightPoint(Vec3d light);
    void setAmbLight(QColor *clr);
    void setDiffLight(QColor* clr);
    void setSpecLight(QColor *clr);
};

#endif // DRAWMANAGER_H
