#ifndef _ISYSTEM_H
#define _ISYSTEM_H

#include <list>
#include <memory>

class IComponent;

class ISystem
{

public:
    ISystem();
    virtual ~ISystem();

    void Update(double _dt);

    bool AddComponent(IComponent& _component);
    bool RemoveComponent(IComponent& _component);
};

#endif