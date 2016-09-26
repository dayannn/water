#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "baseobject.h"

class Camera : public BaseObject
{
public:
    Camera (Vec3d center, double alpha, double beta, double scale);
    ~Camera();

    double getAlpha();
    double getBeta();
    double getScale();
    Vec3d getCenter();

    void setCenter(Vec3d &center);
    void setAlpha(double alpha);
    void setBeta(double beta);

private:
    Vec3d center;
    double alpha;
    double beta;
    double scale;
};

#endif // CAMERA_H
