#include "camera.h"

Camera::Camera(Vec3d center, double alpha, double beta, double scale)
{
    this->center = center;
    this->alpha = alpha;
    this->beta = beta;
    this->scale = scale;
}

Camera::~Camera()
{
}

double Camera::getAlpha()
{
    return this->alpha;
}

double Camera::getBeta()
{
    return this->beta;
}

double Camera::getScale()
{
    return this->scale;
}

Vec3d Camera::getCenter()
{
    return this->center;
}

void Camera::setCenter(Vec3d &center)
{
    this->center = center;
}

void Camera::setAlpha(double alpha)
{
    this->alpha = alpha;
}

void Camera::setBeta(double beta)
{
    this->beta = beta;
}
