#include "stream.h"
#include "model.h"

Stream::Stream(FileStreamImp *imp)
{
    this->imp = imp;
}
Stream::~Stream()
{

}

BaseObject* Stream::loadModel(const std::string *fileName)
{
    Model* model = new Model;
    if (!model)
    {
        throw BadAllocException();
    }

    return this->imp->loadModel(fileName, model);
}

Camera* Stream::loadCamera(const std::string *fileName)
{
    double x, y, z, alpha,beta,scale;
    this->imp->loadCameraParams(fileName, x, y, z, alpha,beta,scale);

    Camera* camera = new Camera(Vec3d{x,y,z},alpha,beta,scale);
    if (!camera)
    {
        throw BadAllocException();
    }

    return camera;
}
