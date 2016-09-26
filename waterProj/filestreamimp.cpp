#include "filestreamimp.h"

FileStreamImp::FileStreamImp()
{

}

FileStreamImp::~FileStreamImp()
{
    if (istream.is_open())
        istream.close();
}

void FileStreamImp::openFile(const std::string *filename)
{
    istream.open (*filename, std::ifstream::in);
    if (istream.fail())
        throw FileOpenException();
}

void FileStreamImp::closeFile()
{
    istream.close();
}

/*BaseObject* FileStreamImp::loadModel(const char *fileName, Model *model)
{

    openFile(fileName);
    if (!this->processPoints(model) || !this->processEdges(model))
    {
        delete model;
        closeFile();
        throw InvalidModelFileException();
    }
    closeFile();

    return model;

}*/

    // EXCEPTIONS TO BE ADDED HERE!!


BaseObject* FileStreamImp::loadModel(const std::string *filename, Model *model)
{
    openFile(filename);
    model->loadFromFile(filename);
    closeFile();

    return model;
}

void FileStreamImp::loadCameraParams(const std::string *fileName, double &x, double &y, double &z, double &alpha, double &beta, double &scale)
{
    openFile(fileName);
   /* if ( !this->processParams(x, y, z, alpha, beta, scale) )
    {
        closeFile();
        throw InvalidCameraFileException();
    }*/
    processParams(x, y, z, alpha, beta, scale);
    closeFile();
}

// catch exceptions? add throws?

void FileStreamImp::readParams(double &alpha, double &beta, double &scale)
{
    istream >> alpha;
    istream >> beta;
    istream >> scale;
}

void FileStreamImp::processParams(double &x, double &y, double &z, double &alpha, double &beta, double &scale)
{
    istream >> x;
    istream >> y;
    istream >> z;
    readParams(alpha, beta, scale);
}
