#ifndef _ENTITY_H
#define _ENTITY_H

#include <list>
#include <memory>

class IComponent;

class Entity
{
private:
    const int id;
    typedef std::list<std::shared_ptr<IComponent>> ComponentList;
    ComponentList m_components;
public:
    Entity();
    ~Entity();

    bool AddComponent(IComponent& _component);
    bool RemoveComponent(IComponent& _component);
};

#endif