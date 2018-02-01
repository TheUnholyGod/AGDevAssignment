#ifndef _STATE_MANAGER_H
#define _STATE_MANAGER_H

#include <bitset>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include "FunctionWrapper.h"

template<typename Base>
class Manager
{
private:
    std::vector<std::shared_ptr<Base>> m_basevector;

    IFunction* m_basefunc;
    std::map<std::shared_ptr<Base>, IFunction*> m_updatefunc;
public:

    Manager(IFunction* _updatefunc)
    {
        m_basefunc = _updatefunc;
    }

    inline std::size_t GetBaseID()
    {
        static std::size_t lastID = 0;
        return ++lastID;
    }

    template <typename Type>
    inline std::size_t GetBaseID() noexcept
    {
        static std::size_t typeID = GetBaseID();
        return typeID;
    }

    template <typename Type>
    inline Base* CreateBase()
    {
        Base* newBase = new Type(GetBaseID<Type>());
        std::cout << "A new thing " << GetBaseID<Type>() << " is being created" << std::endl;
        m_basevector.push_back(std::shared_ptr<Base>(newBase));
        return newBase;
    }

    template <typename Type>
    inline Type* CreateDerived()
    {
        Base* newBase = new Type(GetBaseID<Type>());
        std::cout << "A new thing " << GetBaseID<Type>() << " is being created" << std::endl;
        m_basevector.push_back(std::shared_ptr<Base>(newBase));
        return dynamic_cast<Type*>(newBase);
    }

    void BindFunction(Base* _type, IFunction* _func)
    {
        for (auto &i : m_basevector)
        {
            if (i.get() == _type)
            {
                std::cout << "Func binded" << std::endl;
                m_updatefunc[i] = _func;
            }
        }
    }

    template <typename Type>
    inline Type* GetType()
    {
        for (auto &i : m_basevector)
        {
            if (i->m_id == GetBaseID<Type>())
                return dynamic_cast<Type*>(i.get());
        }
        return dynamic_cast<Type*>(CreateBase<Type>());
    }

    void Update()
    {
        for (auto &i : m_updatefunc)
        {
            i.second->Invoke();
        }
    }

    template<typename Type>
    void Update(Type* _baseobject)
    {
        //(*reinterpret_cast<UpdateFunction*>(m_updatefunc[GetBaseID<Type>()]))(_baseobject);
    }

    template<typename Type>
    void Update()
    {
        for (auto &i : m_basevector)
        {
            if (i->m_id == GetBaseID<Type>())
                Update<Type>(dynamic_cast<Type*>(i.get()));
        }
    }

    void RemoveBase(Base* _base)
    {
        std::vector<std::shared_ptr<Base>>::iterator it = m_basevector.begin();
        do 
        {
            if ((*it).get() == _base)
            {
                m_basevector.erase(it);
                break;
            }
            ++it;
        } while (it != m_basevector.end());

        std::map<std::shared_ptr<Base>,IFunction*>::iterator it1 = m_updatefunc.begin();
        do
        {
            if ((*it1).first.get() == _base)
            {
                m_updatefunc.erase(it1);
                break;
            }
            ++it1;
        } while (it1 != m_updatefunc.end());
    }
};

#endif