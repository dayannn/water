#ifndef TRANSFORMMANAGER_H
#define TRANSFORMMANAGER_H

#include "transformer.h"
#include "baseobject.h"

class TransformManager
{
private:
    Transformer *transformer;

public:
    TransformManager(Transformer *);
    ~TransformManager();

    void transformModel(BaseObject* object, const TransformInfo* transformInfo);
    void transformCamera(BaseObject* object, const TransformInfo* transformInfo);
};

#endif // TRANSFORMMANAGER_H
