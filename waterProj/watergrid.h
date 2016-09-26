#ifndef WATERGRID_H
#define WATERGRID_H

#include "model.h"

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
};

#endif // WATERGRID_H
