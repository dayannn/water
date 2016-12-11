#ifndef WATERGRID_H
#define WATERGRID_H

#include "model.h"
#include <chrono>

using namespace std::chrono;

class WaterGrid : public Model
{
private:


public:
    WaterGrid();
    virtual ~WaterGrid();

    BaseObject* createGrid (double xn, double zn, double xlen, double zlen, double ylevel, int xnum, int znum);
    void recalculateNormals();
    void Solve();

    // убери в private!
    int _xnum;
    int _znum;
    double _dx;
    double _dz;
    Vec3d** prevGrid;
    Vec3d** curGrid;
    Vec3d** nextGrid;


    Vec3d** Hcur;
    Vec3d** HhalfHor;
    Vec3d** HhalfVert;
    Vec3d** Hnew;
    Vec3d** Ucur;
    Vec3d** UhalfHor;
    Vec3d** UhalfVert;
    Vec3d** Unew;
    Vec3d** Vcur;
    Vec3d** VhalfHor;
    Vec3d** VhalfVert;
    Vec3d** Vnew;

    milliseconds prevTime;
};

#endif // WATERGRID_H
