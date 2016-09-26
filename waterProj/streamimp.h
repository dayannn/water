#ifndef STREAMIMP_H
#define STREAMIMP_H

#include "baseexception.h"
#include "baseobject.h"
#include "model.h"
#include "camera.h"


class StreamImp{
public:
    virtual ~StreamImp() {}
    virtual BaseObject* loadModel(const std::string*, Model*) = 0;
    virtual void loadCameraParams(const std::string*, double &,double &,double &, double &,double &,double &) = 0;
};



class EmptySourceStreamException : public BaseException
{
public:
    virtual const char* what()
    {
        return "Source stream is not set!";
    }
};

class FileOpenException : public BaseException
{
public:
    virtual const char* what()
    {
        return "Unable to open file.";
    }
};

class InvalidFileException : public BaseException
{
    virtual const char* what()
    {
        return "File is corrupted or has unsupported format.";
    }
};

class InvalidModelFileException : public InvalidFileException
{
public:
    virtual const char* what()
    {
        return "Model file is corrupted or has unsupported format.";
    }
};

class InvalidCameraFileException : public InvalidFileException
{
public:
    virtual const char* what()
    {
        return "Camera file is corrupted or has unsupported format.";
    }
};

#endif // STREAMIMP_H
