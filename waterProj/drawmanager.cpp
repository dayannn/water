#include "drawmanager.h"
#include "carcasmodelpainter.h"
#include "polygonalmodelpainter.h"
#include "canvas.h"

DrawManager::DrawManager(const CanvasInfo *canvasInfo)
{
    this->canvas = new Canvas(canvasInfo);
    this->painter = new PolygonalModelPainter;
}


DrawManager::~DrawManager()
{
    delete this->canvas;
    delete this->painter;
}


void DrawManager::drawObject(BaseObject *object, Camera *camera)
{
     this->painter->draw(this->canvas, object, camera);
}

void DrawManager::drawObject(CompositeObject *object, Camera *camera)
{
    for (auto it = object->begin(); it != object->end(); it++)
        this->drawObject(*it, camera);
}

void DrawManager::drawScene(BaseScene *scene, vector<Camera *>::iterator &cameraIterator)
{
    this->canvas->clear();
    this->canvas->prepareZBuf();

    for (auto it = scene->compositorBegin(); it != scene->compositorEnd(); it++)
    {
        if ((*it)->addObject(nullptr))
            this->drawObject(static_cast<CompositeObject*>(*it), *cameraIterator);
        else
            this->drawObject(*it, *cameraIterator);
    }
    this->canvas->update();
}

void DrawManager::changeModelType(eModelType type)
{
    delete this->painter;
    switch (type)
    {
    case MODEL_TYPE_CARCASS:
        painter = new CarcasModelPainter;
        break;

    case MODEL_TYPE_POLYGONAL:
        painter = new PolygonalModelPainter;
        break;
    }
}
