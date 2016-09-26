#include "scenemanager.h"

SceneManager::SceneManager(BaseScene *scene)
{
    this->scene = scene;
}

SceneManager::~SceneManager()
{
    delete scene;
}

void SceneManager::clear()
{
    this->scene->clear();
}

vector<Camera*>::iterator& SceneManager::currentCamera()
{
    return camera;
}

vector<BaseObject*>::iterator& SceneManager::currentObject()
{
    return object;
}

vector<BaseObject*>::iterator SceneManager::addObject(BaseObject *object)
{
    return (this->object = this->scene->addObject(object));
}

vector<Camera*>::iterator SceneManager::addCamera(Camera *camera)
{
    return (this->camera = this->scene->addCamera(camera));
}

BaseScene* SceneManager::getScene()
{
    return this->scene;
}
