#pragma once

// includes the lua headers
#include <lua.hpp>
#include <vector>
#include <utility>
#include <typeinfo>
#include <tuple>
#include <algorithm>
#include <typeinfo>
#include <map>

#include "../FunctionWrapper.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();

    std::vector<IFunction*> m_functions;

    std::map<std::string, lua_CFunction> m_functionsforlua;

public:
    // Pointer to the Lua State
    lua_State *theLuaState;

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

    void RegisterFunction()
    {
        lua_CFunction _function;

    }

    static int Test(lua_State* _state)
    {
        return 1;
    }

    template<typename ReturnType>
    struct GenerateFunctionForLua
    {
        lua_CFunction Generate(FunctionWrapper<ReturnType>* _function, std::string _id, int _NoOfReturns)
        {
//            func = Test1;
            std::function<int(lua_State*)>func = [&_function, &_NoOfReturns](lua_State* _state) -> int
            {
                std::cout << "functionInvoked" << std::endl;
                _function->Invoke();
                return _NoOfReturns;
            };
            //s_instance->m_functionsforlua[_id] = (func.target<decltype(func)>());
            lua_register(s_instance->theLuaState, _id.c_str(), s_instance->m_functionsforlua[_id]);
            return s_instance->m_functionsforlua[_id];
        }
    };

    template<typename ReturnType, typename... Variables>
    struct GenerateFunctionForLua<ReturnType(Variables...)>
    {
        lua_CFunction Generate(FunctionWrapper<ReturnType(Variables...)>* _function, std::string _id, int _NoOfReturns)
        {

            std::function<int(lua_State*)>func(
                [&_function, &_NoOfReturns](lua_State* _state) -> int
            {   
                std::tuple<Variables...> functionvars;

                for (int index = 1; index < std::tuple_size<std::tuple<Variables...>>::value; ++index)
                {
                    CLuaInterface::GetInstance()->visit_at(functionvars, index);
                }
                _function->Invoke(functionvars);
                std::cout << "functionInvoked" << std::endl;
                std::cout << "functionInvoked" << std::endl;
                std::cout << "functionInvoked" << std::endl;
                std::cout << "functionInvoked" << std::endl;
                std::cout << "functionInvoked" << std::endl;
                std::cout << "functionInvoked" << std::endl;
                std::cout << "functionInvoked" << std::endl;

                return (_NoOfReturns);
            }
            );
            //std::cout << typeid(static_cast<lua_CFunction>(func.target<int(lua_State*)>())).name() << std::endl;
            //std::cout << typeid(lua_CFunction).name() << std::endl;
            //s_instance->m_functionsforlua[_id] = static_cast<lua_CFunction>(func.target<int(lua_State*)>());
            s_instance->m_functionsforlua[_id] = (func).target<int(lua_State*)>();
            lua_register(s_instance->theLuaState, _id.c_str(), s_instance->m_functionsforlua[_id]);
            return func.target<int(lua_State*)>();
        }
    };

    lua_CFunction RegisterFunction(IFunction* _function)
    {
        
    }

    /*
    ##  Start of FunctionLoader
    */

    template<typename ReturnType>
    struct FunctionLoader
    {
        bool LoadFunction(const char* fileName, const char* funcName)
        {
            if (luaL_loadfile(s_instance->theLuaState, fileName))
            {
                return 0;
            }
            s_instance->m_functions.push_back(new FunctionWrapper<std::vector<ReturnType>()>(
                [this, funcName]() -> std::vector<ReturnType>
            {
                lua_getglobal(s_instance->theLuaState, funcName);

                lua_pcall(s_instance->theLuaState, 0, 1, 0);

                ReturnType ret;
                std::vector<ReturnType> _returnVec;
                while (lua_gettop(s_instance->theLuaState))
                {
                    _returnVec.push_back(s_instance->ConvertTop<decltype(ret)>());
                    lua_pop(s_instance->theLuaState, 1);
                }

                return _returnVec;
            }
            ));
        }
    };

    template<>
    struct FunctionLoader<void>
    {
        bool LoadFunction(const char* fileName, const char* funcName)
        {
            if (!luaL_dofile(s_instance->theLuaState, fileName))
            {
                return 0;
            }
            s_instance->m_functions.push_back(new FunctionWrapper<void>(
                [this, funcName,fileName]()
            {
                if (!luaL_dofile(s_instance->theLuaState, fileName))
                {
                    return 0;
                }
                lua_getglobal(s_instance->theLuaState, funcName);

                lua_pcall(s_instance->theLuaState, 0, 1, 0);
            }
            ));
        }
    };

    template<typename ReturnType, typename... Variables>
    struct FunctionLoader<ReturnType(Variables...)>
    {
        bool LoadFunction(const char* fileName, const char* funcName)
        {
            if (!luaL_dofile(s_instance->theLuaState, fileName))
            {
                return 0;
            }
            s_instance->m_functions.push_back(new FunctionWrapper<std::vector<ReturnType>(Variables...)>(
                [this, funcName,fileName](Variables... _args) -> std::vector<ReturnType>
            {
                if (!luaL_dofile(s_instance->theLuaState, fileName))
                {
                    return 0;
                }
                lua_getglobal(s_instance->theLuaState, funcName);
                std::tuple<Variables...> params = std::forward_as_tuple(_args...);
                s_instance->PushToTop(_args...);

                lua_pcall(s_instance->theLuaState, (int)sizeof...(Variables), 1, 0);

                ReturnType ret;
                std::vector<ReturnType> _returnVec;
                while (lua_gettop(s_instance->theLuaState))
                {
                    _returnVec.push_back(s_instance->ConvertTop<decltype(ret)>());
                    lua_pop(s_instance->theLuaState, 1);
                }

                return _returnVec;
            }
            ));
        }
    };

    template<typename... Variables>
    struct FunctionLoader <void(Variables...)>
    {
        bool LoadFunction(const char* fileName, const char* funcName)
        {
            if (luaL_loadfile(s_instance->theLuaState, fileName))
            {
                return 0;
            }
            s_instance->m_functions.push_back(new FunctionWrapper<void(Variables...)>(
                [this, funcName,fileName](Variables... _args)
            {
                if (luaL_dofile(s_instance->theLuaState, fileName))
                {
                    return 0;
                }
                lua_getglobal(s_instance->theLuaState, funcName);
                std::tuple<Variables...> params = std::forward_as_tuple(_args...);
                s_instance->PushToTop(_args...);

                lua_pcall(s_instance->theLuaState, (int)sizeof...(Variables), 1, 0);
            }
            ));
        }
    };

    /*
    ##  End of FunctionLoader
    */
    
    /*
    ##  Start of ConvertToTop
    */

    template<typename T, typename T1, typename... TArgs>
    T ConvertTop(T _arg, T1 _second, TArgs... _others)
    {
        return lua_gettop(theLuaState, -1);
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

    /*
    ##  End of ConvertToTop
    */

    /*
    ##  Start of PushToTop
    */

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

    /*
    ##  End of PushToTop
    */

    /*
    ##  Start of ReturnByType
    */

 /*   template<typename T, typename T1, typename... TArgs>
    struct ReturnByTypeFull
    {
        std::vector<IFunction*> _initializefunc;
        void ReturnByTypeFunc(int _index)
        {
            _initializefunc.push_back(new FunctionWrapper<T(int)>(&ReturnByType<T>,_index));
            ReturnByTypeFunc<T1, TArgs...>().ReturnByTypeFunc(++_index);
        }
    };*/

    template<typename T>
    T      ReturnByType(int _index)
    {
        return ReturnByType<T>(_index);
    }

    template<>
    int ReturnByType<int>(int _index)
    {
        return lua_tonumber(theLuaState, _index);
    }

    template<>
    bool ReturnByType<bool>(int _index)
    {
        return lua_toboolean(theLuaState, _index);
    }

    template<>
    std::string ReturnByType<std::string>(int _index)
    {
        return lua_tostring(theLuaState,_index);
    }

    /*
    ##  End of ReturnByType
    */

    template <size_t I>
    struct visit_impl
    {
        template <typename T>
        static void visit(T& tup, size_t idx)
        {
            if (idx == I - 1) std::get<I - 1>(tup) = (s_instance->ReturnByType<std::tuple_element<I - 1, T>::type>(I - 1));
            else visit_impl<I - 1>::visit(tup, idx);
        }
    };

    template <>
    struct visit_impl<0>
    {
        template <typename T>
        static void visit(T& tup, size_t idx) 
        {
            // assert(false)
        }
    };

    template <typename... Ts>
    void visit_at(std::tuple<Ts...> const& tup, size_t idx)
    {
        visit_impl<sizeof...(Ts)>::visit(tup, idx);
    }

    template <typename... Ts>
    void visit_at(std::tuple<Ts...>& tup, size_t idx)
    {
        visit_impl<sizeof...(Ts)>::visit(tup, idx);
    }

	// Key to move forward
	char keyFORWARD;
};