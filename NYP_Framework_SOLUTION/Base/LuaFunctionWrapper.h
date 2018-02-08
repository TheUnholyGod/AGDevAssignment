#ifndef _LUA_FUNCTION_WRAPPER
#define _LUA_FUNCTION_WRAPPER

#include <lua.hpp>

#include "Source\FunctionWrapper.h"

struct ILuaFunction
{
    virtual ~ILuaFunction() {}
    virtual int Apply(lua_State* _State) = 0;
};

namespace TuppleManagement
{
    int _lua_dispatcher(lua_State * _State)
    {
        ILuaFunction* func = (ILuaFunction*)lua_touserdata(_State, lua_upvalueindex(1));
        return func->Apply(_State);
    }

    template<size_t... Is>
    struct _index {};

    template<size_t N,size_t... Is>
    struct _index_builder : _index_builder<N - 1, N - 1, Is...> {};

    template<size_t... Is>
    struct _index_builder<0, Is...>
    {
        using type = _index<Is...>;
    };

    template<typename... T, size_t... N>
    std::tuple<T...> _get_args(lua_State* _State, _index<N...>)
    {
        return std::make_tuple<T...>(_check_top<T>(_State, N + 1)...);
    }

    template<typename... T>
    std::tuple<T...> _get_args(lua_State* _State)
    {
        constexpr size_t argCount = sizeof...(T);
        return _get_args<T...>(_State, typename _index_builder<argCount>::type());
    }

    template<typename T>
    T _check_top(lua_State* _State, int _index)
    {
        return _check_top<T>(_State, _index);
    }

    template<>
    int _check_top<int>(lua_State* _State, int _index)
    {
        if (lua_type(_State, _index) == LUA_TNUMBER)
            return lua_tointeger(_State, _index);
        return 0;
    }

    template<>
    bool _check_top<bool>(lua_State* _State, int _index)
    {
        if (lua_type(_State, _index) == LUA_TBOOLEAN)
            return lua_toboolean(_State, _index);
        return false;
    }

    template<>
    std::string _check_top<std::string>(lua_State* _State, int _index)
    {
        if(lua_type(_State,_index) == LUA_TSTRING)
            return lua_tostring(_State, _index);
        return "";
    }

    template<typename ReturnType,typename... Args,size_t... N>
    ReturnType _unpack_and_call(std::function < ReturnType(Args...)> _func, std::tuple<Args...> _args, _index<N...>)
    {
        return _func(std::get<N>(_args)...);
    }

    template<typename ReturnType, typename... Args>
    ReturnType _unpack_and_call(std::function < ReturnType(Args...)> _func, std::tuple<Args...> _args)
    {
        return _unpack_and_call(_func, _args, typename _index_builder<sizeof...(Args)>::type());
    }

    template <typename T>
    void _push_to_top(lua_State* _State, T _value)
    {
        _push_to_top<T>(_State, _value);
    }

    template <>
    void _push_to_top<int>(lua_State* _State, int _value)
    {
        lua_pushnumber(_State, _value);
    }

    template <>
    void _push_to_top<bool>(lua_State* _State, bool _value)
    {
        lua_pushboolean(_State, _value);
    }

    template <>
    void _push_to_top<std::string>(lua_State* _State, std::string _value)
    {
        lua_pushstring(_State, _value.c_str());
    }
}

template<int N, typename ReturnType, typename... Args>
class LuaFunctionWrapper : public FunctionWrapper<ReturnType(Args...)>,public ILuaFunction
{
private:
    lua_State** m_State;
public:
    LuaFunctionWrapper(lua_State** _State, const std::string& _ID, ReturnType(*_func)(Args...)) :
        LuaFunctionWrapper(_State,_ID, FunctionPointer(_func)) {}

    LuaFunctionWrapper(lua_State** _State, const std::string& _ID, FunctionPointer _func) :
        FunctionWrapper<ReturnType(Args...)>(_ID, _func), m_State(_State) 
    {
        lua_pushlightuserdata(*m_State, (void*)static_cast<ILuaFunction*>(this));
        lua_pushcclosure(*m_State, &TuppleManagement::_lua_dispatcher, 1);
        lua_setglobal(*m_State, m_ID.c_str());
    }

    ~LuaFunctionWrapper()
    {
        if (!m_State && !*m_State)
        {
            lua_pushnil(*m_State);
            lua_setglobal(*m_State, m_ID.c_str());
        }
    }

    virtual int Apply(lua_State* _State)
    {
        std::tuple<Args...> args = TuppleManagement::_get_args<Args...>(_State);
        ReturnType value = TuppleManagement::_unpack_and_call(m_function, args);
        TuppleManagement::_push_to_top(_State, value);
        return 1;
    }
};

#endif