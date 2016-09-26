#ifndef BASESCENE_H
#define BASESCENE_H

#include "camera.h"
#include "baseobject.h"
#include <vector>
using std::vector;


class BaseScene
{
public:
    virtual ~BaseScene() {}

    virtual void clear() = 0;

    virtual vector<Camera*>::iterator  addCamera(Camera*) = 0;
    virtual vector<BaseObject*>::iterator  addObject(BaseObject*) = 0;

    virtual vector<Camera*>::iterator cameraBegin() = 0;
    virtual vector<Camera*>::iterator cameraEnd() = 0;

    virtual vector<BaseObject*>::iterator compositorBegin() = 0;
    virtual vector<BaseObject*>::iterator compositorEnd() = 0;
};


#endif // BASESCENE_H
