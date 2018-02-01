#pragma once

// includes the lua headers
#include <lua.hpp>
#include <vector>
#include <utility>
#include <typeinfo>
#include <tuple>
#include <algorithm>

#include "../FunctionWrapper.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();

    std::vector<IFunction*> m_functions;

public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			// Drop the Lua Interface Class
			s_instance->Drop();

			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	virtual ~CLuaInterface();

	// Initialisation of the Lua Interface Class
	bool Init();

	// Run the Lua Interface Class
	void Run();

	// Drop the Lua Interface Class
	void Drop();

	// Get an integer value through the Lua Interface Class
	int getIntValue(const char* varName);
	// Get a float value through the Lua Interface Class
	float getFloatValue(const char* varName);

	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);

    template<typename ReturnType, typename... Variables>
    bool LoadFunction(const char* fileName, const char* funcName)
    {
        if (luaL_loadfile(theLuaState, fileName))
        {
            return 0;
        }
        m_functions.push_back(new FunctionWrapper<std::vector<ReturnType>(Variables...)>(
            [this, funcName](Variables... _args) -> std::vector<ReturnType>
        {
            lua_getglobal(theLuaState, funcName);
            std::tuple<Variables...> params = std::forward_as_tuple(_args...);
            PushToTop(_args...);

            lua_pcall(theLuaState, (int)sizeof...(Variables), 1, 0);

            ReturnType ret;
            std::vector<ReturnType> _returnVec;
            while (lua_gettop(theLuaState))
            {
                _returnVec.push_back(ConvertTop<decltype(ret)>());
                lua_pop(theLuaState, 1);
            }

            return _returnVec;
        }
        ));
    }

    template<typename T>
    T ConvertTop()
    {
        return lua_gettop(theLuaState, -1);
    }

    template<>
    int ConvertTop<int>()
    {
        return lua_tonumber(theLuaState, -1);
    }

    template<>
    bool ConvertTop<bool>()
    {
        return lua_toboolean(theLuaState, -1);
    }

    template<>
    std::string ConvertTop<std::string>()
    {
        return lua_tostring(theLuaState, -1);
    }

    template<typename T,typename T1,typename... TArgs>
    void PushToTop(T _arg,T1 _second,TArgs... _others)
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
        lua_pushnumber(theLuaState, _arg);
    }

    template<>
    void PushToTop<bool>(bool _arg)
    {
        lua_pushboolean(theLuaState, _arg);
    }

    template<>
    void PushToTop<std::string>(std::string _arg)
    {
        lua_pushstring(theLuaState, _arg.c_str());
    }

	// Pointer to the Lua State
	lua_State *theLuaState;

	// Key to move forward
	char keyFORWARD;
};

