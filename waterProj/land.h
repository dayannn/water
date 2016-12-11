#ifndef LAND_H
#define LAND_H

#include "model.h"

class Land : public Model
{
private:
    int _xnum;
    int _znum;
    double _xn;
    double _zn;
    double _xlen;
    double _zlen;
    double _dx;
    double _dz;


public:
    Land();
    virtual ~Land();

    BaseObject* createGrid ();
    void recalculateNormals();
    double getHeight(double x, double z);

};

#endif // LAND_H
