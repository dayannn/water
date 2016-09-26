#include "transformer.h"
#include "matrix.h"

Transformer::Transformer()
{
}

#include "polygonalmodelpainter.h"


// лишнее?
void Transformer::transformPoint(Vec3d* const point, const TransformInfo *transformInfo) const
{
    Matrix matrix;

    switch(transformInfo->type)
    {
   /* case TRANSFORM_ROTATE_X:
        Matrix::rotateX(matrix, convertRadToDeg(transformInfo->delta));
                break;
    case TRANSFORM_ROTATE_Y:
        Matrix::rotateY(matrix, convertRadToDeg(transformInfo->delta));
                break;
    case TRANSFORM_ROTATE_Z:
        Matrix::rotateZ(matrix, convertRadToDeg(transformInfo->delta));
                break;*/

    case TRANSFORM_MOVE_X:
        point->setX(point->x + transformInfo->delta);
        return;     // хммммм
    case TRANSFORM_MOVE_Y:
        point->setY(point->y + transformInfo->delta);
        return;
    case TRANSFORM_MOVE_Z:
        point->setZ(point->z + transformInfo->delta);
        return;

    case TRANSFORM_SCALE:
        point->setX(point->x * transformInfo->delta);
        point->setY(point->y * transformInfo->delta);
        point->setZ(point->z * transformInfo->delta);
        break;
    default:
        throw InvalidTransformException();
        break;
    }

    Vec3d coord;
    coord.x = point->x;
    coord.y = point->y;
    coord.z = point->z;

    matrix = matrix * Matrix(coord);
    point->setX(coord[0]);
    point->setY(coord[1]);
    point->setZ(coord[2]);
}

Transformer::~Transformer()
{

}

void Transformer::transformParams(double *delta, double *beta, double *scale) const
{
    *delta *= this->delta;
    *beta  *= this->beta;
    *scale *= this->scale;
}

void Transformer::rotate_camera(Camera *camera, const TransformInfo *info)
{
    if (info->type == TRANSFORM_ROTATE_X)
    {
        double alpha = camera->getAlpha();
        alpha += info->delta;
        camera->setAlpha(alpha);
    }
    else if (info->type == TRANSFORM_ROTATE_Y)
    {
        double beta = camera->getBeta();
        beta += info->delta;
        camera->setBeta(beta);
    }
}
