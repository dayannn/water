#ifndef SCENE_H
#define SCENE_H

#include "basescene.h"
#include "compositeobject.h"

class Scene : public BaseScene
{
private:
    CompositeObject compositor;
    std::vector<Camera*> cameraList;

public:
    Scene();
    ~Scene();

    virtual void clear() override;

    virtual  vector<BaseObject*>::iterator addObject(BaseObject *) override;
    virtual  vector<Camera*>::iterator addCamera(Camera *) override;

    virtual  vector<Camera*>::iterator cameraBegin() override;
    virtual  vector<Camera*>::iterator cameraEnd() override;

    virtual  vector<BaseObject*>::iterator compositorBegin() override;
    virtual  vector<BaseObject*>::iterator compositorEnd() override;
};

#endif // SCENE_H
