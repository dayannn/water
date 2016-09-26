#ifndef LOADMANAGER_H
#define LOADMANAGER_H

#include "stream.h"
#include "baseobject.h"
#include "model.h"
#include "camera.h"

class LoadManager
{
public:
    LoadManager(Stream* stream);
    ~LoadManager();

    BaseObject* loadObject(const StreamInfo* streamInfo);
    Camera* loadCamera(const StreamInfo* streamInfo);

private:
    Stream *stream;
};

#endif // LOADMANAGER_H
