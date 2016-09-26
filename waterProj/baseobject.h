#ifndef BASEOBJECT_H
#define BASEOBJECT_H

enum eModelType {MODEL_TYPE_CARCASS, MODEL_TYPE_POLYGONAL};

class BaseObject{
public:
    virtual ~BaseObject() {}
    virtual bool addObject(BaseObject *){ return false; } // ??????
};

#endif // BASEOBJECT_H
