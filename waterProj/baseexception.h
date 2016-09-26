#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <exception>

class BaseException : public std::exception
{
public:
    virtual  const char* what()
    {
       return "Unexpected exception occured.";
    }
};

#endif // BASEEXCEPTION_H
