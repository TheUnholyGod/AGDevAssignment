#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "IComponent.h"

template<typename Type,typename System>
class Component : public Type
{
private:
    //data here
public:
    Component();
    ~Component();

    bool GetData(void* _dataretriver);
};

#endif