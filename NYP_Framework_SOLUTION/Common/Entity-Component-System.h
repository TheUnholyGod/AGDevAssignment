#ifndef _ECS_H
#define _ECS_H

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Entity;
class Component;

using ComponentTypeID = std::size_t;

inline ComponentTypeID GetComponentTypeID()
{
    static ComponentTypeID lastID = 0;
    return ++lastID;
}

template <typename ComponentType>
inline ComponentTypeID GetComponentTypeID() noexcept
{
    static ComponentTypeID typeID = GetComponentTypeID();
    return typeID;
}

constexpr std::size_t MaxComponents = 32;
using ComponentBitSet = std::bitset<MaxComponents>;
using ComponentArray = std::array<Component*, MaxComponents>;

class Component
{
private: 
    Entity* m_parent;
public:
    virtual ~Component() {}

    virtual void Init() {}
    virtual void Update() {}
    virtual void Render() {}

};

class Entity
{
private:
    bool m_active;
    std::vector<std::unique_ptr<Component>> m_components;

    ComponentArray m_component_array;
    ComponentBitSet m_component_bitset;

public:
    void Update()
    {
        for (std::vector<std::unique_ptr<Component>>::iterator it = m_components.begin();
            it != m_components.end();
            ++it)
            it->get()->Update();
    }

    void Render()
    {
        for (std::vector<std::unique_ptr<Component>>::iterator it = m_components.begin();
            it != m_components.end();
            ++it)
            it->get()->Render();
    }

    bool IsActive() const
    {
        return this->m_active;
    }

    void Destroy()
    {
        this->m_active = false;
    }

    template <typename ComponentType> bool HasComponent() const
    {
        return this->m_component_bitset[GetComponentTypeID<ComponentType>()];
    }

    template <typename ComponentType,typename... ComponentArgs>
    ComponentType& AddComponent(ComponentArgs&&... _components)
    {
        ComponentType* c(new ComponentType(std::forward<ComponentArgs>(_components)...));
        c->m_parent = this;
        std::unique_ptr<Component> uPtr{ c };
        this->m_components.emplace_back(std::move(uPtr));

        this->m_component_array[GetComponentTypeID<ComponentType>()] = c;
        this->m_component_bitset[GetComponentTypeID<ComponentType>()] = true;
        c->Init();
        return *c;
    }

    template <typename ComponentType>
    ComponentType& GetComponent() const
    {
        Component* ptr = this->m_component_array[GetComponentTypeID<ComponentType>()];
        return static_cast<ComponentType*>(ptr);
    }
};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> m_entites;
public:
    void Update()
    {
        for (std::vector<std::unique_ptr<Entity>>::iterator it = m_entites.begin();
            it != m_entites.end();
            ++it)
            it->get()->Update();
    }

    void Render()
    {
        for (std::vector<std::unique_ptr<Entity>>::iterator it = m_entites.begin();
            it != m_entites.end();
            ++it)
            it->get()->Render();
    }

    void Refresh()
    {
        m_entites.erase(std::remove_if(std::begin(m_entites), std::end(m_entites),
            [](const std::unique_ptr<Entity>& _entity)
        {
            return !_entity->IsActive();
        }),std::end(m_entites));
    }

    Entity& AddEntity()
    {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{ e };
        m_entites.emplace_back(std::move(uPtr));
        return *e;
    }
};

#endif