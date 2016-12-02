#ifndef LAND_H
#define LAND_H

#include "model.h"

class Land : public Model
{
private:

public:
    Land();
    virtual ~Land();

    BaseObject* createGrid ();
    void recalculateNormals();


};

#endif // LAND_H
