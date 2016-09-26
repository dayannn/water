#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "basescene.h"

class SceneManager
{
private:
    BaseScene *scene;
    vector<Camera*>::iterator camera;
    vector<BaseObject*>::iterator object;

public:
    SceneManager(BaseScene *);
    ~SceneManager();

    vector<BaseObject*>::iterator addObject(BaseObject *);
    vector<Camera*>::iterator addCamera(Camera *);

    vector<Camera*>::iterator& currentCamera();
    vector<BaseObject*>::iterator& currentObject();

    BaseScene *getScene();

    void clear();
};

#endif // SCENEMANAGER_H
