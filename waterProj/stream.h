#ifndef STREAM_H
#define STREAM_H

#include "baseobject.h"
#include "camera.h"
#include "exceptions.h"

#include "streamimp.h"
#include "filestreamimp.h"

enum eSourceType
{
    SOURCE_UNDEFINED,
    SOURCE_FILE
};

struct StreamInfo
{
    eSourceType sourceType;
    std::string *sourceName;
};

class Stream
{
public:
    Stream(FileStreamImp *);
    ~Stream();

    BaseObject* loadModel(const std::string *);
    Camera* loadCamera(const std::string *);

private:
    StreamImp *imp;
};








#endif // STREAM_H
