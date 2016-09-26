#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "baseexception.h"

class BadAllocException : public BaseException
{
    virtual const char* what()
    {
        return "Memory allocation error!";
    }
};

class FileNotFoundException : public BaseException
{
    virtual const char* what()
    {
        return "File not found/unable to open file!";
    }
};

class FileCorruptedException : public BaseException
{
    virtual const char* what()
    {
        return "File is corrupted or has an unsupported format!";
    }
};

#endif // EXCEPTIONS_H
