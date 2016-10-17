#include "transformmanager.h"
#include "model.h"
#include "camera.h"

using std::vector;

TransformManager::TransformManager(Transformer *transformer)
{
    this->transformer = transformer;
}
TransformManager::~TransformManager()
{
    delete this->transformer;
}

void TransformManager::transformModel(BaseObject* object, const TransformInfo *transformInfo)
{

    Model *model = static_cast<Model*>(object);


    switch(transformInfo->type)
    {
    case TRANSFORM_ROTATE_X:
        model->getPositionInfo().alpha += transformInfo->delta;
        break;

    case TRANSFORM_ROTATE_Y:
        model->getPositionInfo().beta += transformInfo->delta;
        break;

    case TRANSFORM_ROTATE_Z:
        model->getPositionInfo().gamma += transformInfo->delta;
        break;

    case TRANSFORM_MOVE_X:
        model->getPositionInfo().x += transformInfo->delta;
        break;

    case TRANSFORM_MOVE_Y:
        model->getPositionInfo().y += transformInfo->delta;
        break;

    case TRANSFORM_MOVE_Z:
        model->getPositionInfo().z += transformInfo->delta;
        break;

    default:
        throw InvalidTransformException();
        break;
    }


}

void TransformManager::transformCamera(BaseObject* object, const TransformInfo* transformInfo)
{
    Camera *camera = static_cast<Camera*>(object);
    if (transformInfo->type == TRANSFORM_MOVE_X || transformInfo->type == TRANSFORM_MOVE_Y
            || transformInfo->type == TRANSFORM_MOVE_Z)
    {
        Vec3d p = camera->getCenter();
        transformer->transformPoint(&p, transformInfo);
        camera->setCenter(p);
    }
    else if (transformInfo->type == TRANSFORM_ROTATE_X || transformInfo->type == TRANSFORM_ROTATE_Y)
    {
        transformer->rotate_camera(camera, transformInfo);
    }
    else if (transformInfo->type == TRANSFORM_MOVE_FORWARD)  // движение вперед/назад относительно направления взгляда камеры
    {
        Vec3d p = camera->getCenter();

        p.x = (p.x + transformInfo->delta*sin(camera->getBeta()));
        p.z = (p.z + transformInfo->delta*(-cos(camera->getBeta())));
/*Vec3d camdir (cos(camera->getAlpha())*sin(camera->getBeta()), -sin(camera->getAlpha()), -cos(camera->getAlpha())*cos(camera->getBeta()));

        p.setX(p.x + transformInfo->delta*cos(camera->getAlpha())*sin(camera->getBeta()));
        p.setY(p.y + transformInfo->delta*(-sin(camera->getAlpha())));
        p.setZ(p.z + transformInfo->delta*(-cos(camera->getAlpha())*cos(camera->getBeta())));*/

        camera->setCenter(p);
    }
    else if (transformInfo->type == TRANSFORM_MOVE_SIDE) // движение вправо/влево относительно направления взгляда камеры
    {
        Vec3d p = camera->getCenter();
        p.x = (p.x + transformInfo->delta*cos(camera->getBeta()));
        p.z = (p.z + transformInfo->delta*sin(camera->getBeta()));
        camera->setCenter(p);
    }

}
