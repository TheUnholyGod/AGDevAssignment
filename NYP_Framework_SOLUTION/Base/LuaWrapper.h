#ifndef _LUA_WRAPPER_H
#define _LUA_WRAPPER_H

#include <lua.hpp>

#include <string>
#include <tuple>
#include <memory>
#include <map>

#include "LuaFunctionWrapper.h"

class LuaState
{
private:
    lua_State* m_luaState;
    std::map<std::string, std::unique_ptr<ILuaFunction>> m_functions;
public:
    LuaState() : m_luaState(nullptr)
    {
        m_luaState = luaL_newstate();
        m_luaState = lua_open();
        luaL_openlibs(m_luaState);

    }
    
    ~LuaState()
    {
        if (!m_luaState)
            return;

        lua_close(m_luaState);
    }

    template<typename ReturnType,typename... Args>
    void Register(const std::string& _name, std::function<ReturnType(Args...)> _func)
    {
        auto temp = std::unique_ptr<ILuaFunction> 
        {
            new LuaFunctionWrapper<1,ReturnType,Args...>(&m_luaState,_name,_func)
        };
        m_functions[_name] = std::move(temp);
    }

    void RunScript(std::string _filename)
    {
        luaL_dofile(m_luaState, _filename.c_str());
    }

    /*
    ##  Start of PushToTop
    */
    template<typename T, typename T1, typename... TArgs>
    void PushToTop(T _arg, T1 _second, TArgs... _others)
    {
        PushToTop<T>(_arg);
        PushToTop<T1, TArgs...>(_second, _others...);
    }

    template<typename T>
    void PushToTop(T _arg)
    {
        PushToTop<T>(_arg);
    }

    template<>
    void PushToTop<int>(int _arg)
    {
        lua_pushnumber(m_luaState, _arg);
    }

    template<>
    void PushToTop<bool>(bool _arg)
    {
        lua_pushboolean(m_luaState, _arg);
    }

    template<>
    void PushToTop<std::string>(std::string _arg)
    {
        lua_pushstring(m_luaState, _arg.c_str());
    }
    /*
    ##  End of PushToTop
    */

    /*
    ##  Start of ReturnByType
    */

    template<typename T>
    T Read(int _index)
    {
        return ReturnByType<T>(_index);
    }

    template<>
    int Read<int>(int _index)
    {
        return lua_tointeger(m_luaState, _index);
    }

    template<>
    bool Read<bool>(int _index)
    {
        return lua_toboolean(m_luaState, _index);
    }

    template<>
    std::string Read<std::string>(int _index)
    {
        return lua_tostring(m_luaState, _index);
    }

    /*
    ##  End of ReturnByType
    */


    /*
    ##  Start of Pop
    */
    
    template <typename T>
    void PopBottom(LuaState& _State)
    {
        T returninfo = _State.Read<T>(1);
        lua_remove(_State.m_luaState, 1);
        return returninfo;
    }

    template <size_t,typename... Ts>
    struct _pop
    {
        template<typename T>
        static std::tuple<T> MakeTuple(const LuaState& _State,const int _Index)
        {
            return std::make_tuple(_State.Read<T>(_Index));
        }

        template<typename T,typename T1,typename... Rest>
        static std::tuple<T, T1, Rest...> MakeTuple(const LuaState& _State, const int _Index)
        {
            auto head = std::make_tuple(_State.Read(_index));
            return std::tuple_cat(head, MakeTuple<T1, Rest...>(_State, _Index));
        }

        static std::tuple<Ts...> Apply(LuaState& _State)
        {
            auto returninfo = MakeTuple(_State, 1);
            lus_pop(_State.m_luaState, sizeof...(Ts));
            return returninfo;
        }
    };

    template <typename T>
    struct _pop<1,T>
    {
        static T Apply(LuaState& _State)
        {
            T returninfo = _State.Read<T>(-1);
            lua_pop(_State.m_luaState, 1);
            return returninfo;
        }
    };

    template <typename... Ts>
    struct _pop<0, Ts...>
    {
        static void Apply(LuaState& _State)
        {
            
        }
    };

    template <typename... ReturnType,typename... Args>
    typename _pop<sizeof...(ReturnType),ReturnType...> Call(const std::string& _FileName, const std::string& _FunctionName,const Args&... _Args)
    {
        lua_getglobal(m_luaState, _FunctionName.c_str());

        const int argsCount = sizeof...(Args);
        const int retCount = sizeof...(ReturnType);

        PushToTop(_Args...);
        lua_call(m_luaState, argsCount, retCount);

        return _pop<sizeof...(ReturnType),ReturnType...>::Apply(*this);
        lua_pushlightuserdata
    };

    /*
    ##  Start of Pop
    */
};

#endif