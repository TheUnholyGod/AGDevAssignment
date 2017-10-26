#ifndef _ISYSTEM_H
#define _ISYSTEM_H

#include <list>
#include <memory>

#include "ISystem.h"

template <class Type,class System>
class Component;

template<typename Component_Type>
class System : public ISystem
{
private:
    typedef std::list<std::shared_ptr<Component<Component_Type, System<Component_Type>>>> ComponentList;
    ComponentList m_components;
public:
    ISystem();
    virtual ~ISystem();

    void Update(double _dt);

    bool AddComponent(IComponent& _component);
    bool RemoveComponent(IComponent& _component);
};

#endif