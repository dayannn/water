#include "compositeobject.h"

CompositeObject::CompositeObject() {}

CompositeObject::~CompositeObject()
{
    this->removeAll();
}

size_t CompositeObject::getSize()
{
    return this->objects.size();
}

bool CompositeObject::addObject(BaseObject *object)
{
    if (object)
    {
        this->objects.insert(objects.begin(), object);
                //push_front(object);
    }
    return true;
}

void CompositeObject::removeAll()
{
    vector<BaseObject*>::iterator it;

    for (it = objects.begin(); it != objects.end(); it++)
        delete *it;

    if (this->getSize())
    {
        this->objects.clear();
    }
}

vector<BaseObject*>::iterator CompositeObject::begin()
{
    vector<BaseObject*>::iterator iterator(objects.begin());
    return iterator;
}

vector<BaseObject*>::iterator CompositeObject::end()
{
    vector<BaseObject*>::iterator iterator(objects.end());
    return iterator;
}
