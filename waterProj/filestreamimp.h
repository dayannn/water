#ifndef FILESTREAMIMP_H
#define FILESTREAMIMP_H

#include "streamimp.h"
#include "fstream"
#include "sstream"
#include <stdio.h>

class FileStreamImp : public StreamImp
{
public:
    FileStreamImp();
    ~FileStreamImp();
    virtual BaseObject* loadModel(const std::string *filename, Model *model) override;
    virtual void loadCameraParams(const std::string *fileName, double &x, double &y, double &z, double &alpha, double &beta, double &scale) override;

private:
    std::ifstream istream;

    void readParams(double &alpha, double &beta, double &scale);
    void processParams(double &,double &,double &, double &,double &,double &);

    void openFile(const std::string *filename);
    void closeFile();
};

#endif // FILESTREAMIMP_H
