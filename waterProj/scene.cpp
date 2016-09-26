#include "scene.h"

Scene::Scene(){}

Scene::~Scene()
{
    this->clear();
}

void Scene::clear()
{
    this->compositor.removeAll();
    this->cameraList.clear();
}

vector<BaseObject*>::iterator Scene::addObject(BaseObject *object)
{
    this->compositor.addObject(object);
    return this->compositor.begin();
}

vector<Camera*>::iterator Scene::addCamera(Camera *camera)
{
    this->cameraList.insert(cameraList.begin(), camera);
    vector<Camera*>::iterator iterator(cameraList.begin());

    return iterator;
}

vector<Camera*>::iterator Scene::cameraBegin()
{
    vector<Camera*>::iterator iterator(cameraList.begin());
    return iterator;
}

vector<Camera*>::iterator Scene::cameraEnd()
{
    vector<Camera*>::iterator iterator(cameraList.end());
    return iterator;
}

vector<BaseObject*>::iterator Scene::compositorBegin()
{
    return compositor.begin();
}

vector<BaseObject*>::iterator Scene::compositorEnd()
{
    return compositor.end();
}
