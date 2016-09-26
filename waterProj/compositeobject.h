#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "baseobject.h"
#include <vector>
using std::size_t;
using std::vector;

class CompositeObject : public BaseObject
{
private:
    std::vector<BaseObject*> objects;

public:
    CompositeObject();
    ~CompositeObject();

    virtual bool addObject(BaseObject*) override;
    void removeAll();

    vector<BaseObject*>::iterator begin();
    vector<BaseObject*>::iterator end();

    size_t getSize();

};

#endif // COMPOSITEOBJECT_H
