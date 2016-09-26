#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "baseexception.h"
#include "camera.h"

//const double M_PI = 3.14159265358979323846;

enum TransformType
{
    TRANSFORM_ROTATE_X,
    TRANSFORM_ROTATE_Y,
    TRANSFORM_ROTATE_Z,
    TRANSFORM_MOVE_X,
    TRANSFORM_MOVE_Y,
    TRANSFORM_MOVE_Z,
    TRANSFORM_MOVE_FORWARD,
    TRANSFORM_MOVE_SIDE,
    TRANSFORM_SCALE
};
struct TransformInfo
{
    TransformType type;
    double delta;
};

class InvalidTransformException : public BaseException
{
public:
    virtual const char* what()
    {
        return "Transform type is not set!";
    }
};

class Transformer
{
public:
    Transformer();
    ~Transformer();

    void transformPoint(Vec3d* const p, const TransformInfo *) const;
    void transformParams(double*,double*,double*) const;
    void rotate_camera (Camera *camera, const TransformInfo* info);

    static double convertRadToDeg(double angle) { return (angle * M_PI) / 180.0; }

private:
    double delta;
    double beta;
    double scale;
};

#endif // TRANSFORMER_H
