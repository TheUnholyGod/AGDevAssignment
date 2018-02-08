#ifndef _FUNCTION_WRAPPER_H
#define _FUNCTION_WRAPPER_H

#include <functional>
#include <tuple>
#include <utility>
#include <string>

class IFunction
{
private:
    std::string m_ID;
public:
    IFunction() {}
    IFunction(std::string _ID) :m_ID(_ID) {}
	virtual ~IFunction() = 0;
    virtual void Invoke() {};
    std::string GetID() { return m_ID; }
    virtual IFunction* DownCast() { return this; };
};

template <typename ret>
class IFunction2
{
public:
    //virtual ret Invoke() = 0;
};

template <>
class IFunction2<void>
{
public:
    //virtual void Invoke() = 0;
};

template<typename ret> 
class FunctionWrapper : public IFunction,public IFunction2<ret>
{
private:
	typedef std::function<ret()> FunctionPointer;
	FunctionPointer m_function;
public:
    FunctionWrapper <ret>(std::string _ID) :IFunction(_ID) {}
	FunctionWrapper <ret>(FunctionPointer _function) : m_function(_function) {}

    virtual ~FunctionWrapper() {}

    virtual void Invoke()
	{
		m_function();
	}

    virtual ret(*ReturnFunction())()
    {
        return m_function.target<ret()>();
    }
};

template<typename ret, typename... params>
class FunctionWrapper <ret(params...)>: public IFunction,public IFunction2<ret>
{
private:
	typedef std::function<ret(params...)> FunctionPointer;
	typedef std::tuple<params...> Parameters;
	FunctionPointer m_function;
	Parameters m_parameterpack;

	template <bool Done, int Total, int... N>
	struct Unpacker
	{
		static ret call(FunctionPointer f, Parameters && t)
		{
			return Unpacker<Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Parameters>(t));
		}
	};

	template <int Total, int... N>
	struct Unpacker<true, Total, N...>
	{
		static ret call(FunctionPointer f, Parameters && t)
		{
			return f(std::get<N>(std::forward<Parameters>(t))...);
		}
	};

public:
    FunctionWrapper <ret(params...)>(std::string _ID, FunctionPointer _function) : IFunction(_ID), m_function(_function) {}

	FunctionWrapper <ret(params...)>(FunctionPointer _function) : m_function(_function)
	{
        m_parameterpack = std::tuple<params...>();
	}

	FunctionWrapper <ret(params...)>(FunctionPointer _function, params&&... _params) : m_function(_function)
	{
		m_parameterpack = std::forward_as_tuple(_params...);
	}

	virtual ~FunctionWrapper<ret(params...)>() {}

    virtual void Invoke()
    {
        Unpacker<0 == std::tuple_size<Parameters>::value, std::tuple_size<Parameters>::value>::call(m_function, std::forward<Parameters>(m_parameterpack));
    }

    virtual ret Invoke(params... _params)
	{
		return m_function(_params...);
	}

    virtual ret Invoke(std::tuple<params...> _params)
    {
        return Unpacker<0 == std::tuple_size<Parameters>::value, std::tuple_size<Parameters>::value>::call(m_function, std::forward<Parameters>(_params));
    }

    virtual ret(*GetCPointer())(params...)
    {
        return (*m_function.target<ret(*)(params...)>());
    }

    template <typename Var>
    bool SetParameter(Var _arg, int _index = 0)
    {
        if (_index > std::tuple_size<Parameters>::value)
            return false;
        if (std::tuple_element<_index, Parameters> != decltype(_arg))
            return false;
        std::get<_index, m_parameterpack> ref = _arg;
        return true;
    }

    virtual FunctionWrapper <ret(params...)>* DownCast()
    {
        return static_cast<FunctionWrapper <ret(params...)>*>(this);
    }

};

template<typename... params>
class FunctionWrapper <void(params...)> : public IFunction, public IFunction2<void>
{
private:
    typedef std::function<void(params...)> FunctionPointer;
    typedef std::tuple<params...> Parameters;
    FunctionPointer m_function;
    Parameters m_parameterpack;

    template <bool Done, int Total, int... N>
    struct Unpacker
    {
        static void call(FunctionPointer f, Parameters && t)
        {
            Unpacker<Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Parameters>(t));
        }
    };

    template <int Total, int... N>
    struct Unpacker<true, Total, N...>
    {
        static void call(FunctionPointer f, Parameters && t)
        {
            f(std::get<N>(std::forward<Parameters>(t))...);
        }
    };

public:
    FunctionWrapper <void(params...)>(std::string _ID, FunctionPointer _function) : IFunction(_ID), m_function(_function) {}

    FunctionWrapper <void(params...)>(FunctionPointer _function) : m_function(_function)
    {
        m_parameterpack = std::tuple<params...>();
    }

    FunctionWrapper <void(params...)>(FunctionPointer _function, params&&... _params) : m_function(_function)
    {
        m_parameterpack = std::forward_as_tuple(_params...);
    }

    virtual ~FunctionWrapper<void(params...)>() {}

    virtual void Invoke()
    {
        Unpacker<0 == std::tuple_size<Parameters>::value, std::tuple_size<Parameters>::value>::call(m_function, std::forward<Parameters>(m_parameterpack));
    }

    virtual void Invoke(params... _params)
    {
        m_function(_params...);
    }

    virtual void Invoke(std::tuple<params...> _params)
    {
        Unpacker<0 == std::tuple_size<Parameters>::value, std::tuple_size<Parameters>::value>::call(m_function, std::forward<Parameters>(_params));
    }

    virtual void(*GetCPointer())(params...)
    {
        return (*m_function.target<void(*)(params...)>());
    }

    template <typename Var>
    bool SetParameter(Var _arg, int _index = 0)
    {
        if (_index > std::tuple_size<Parameters>::value)
            return false;
        if (std::tuple_element<_index, Parameters> != decltype(_arg))
            return false;
        std::get<_index, m_parameterpack> ref = _arg;
        return true;
    }

    virtual FunctionWrapper <void(params...)>* DownCast()
    {
        return static_cast<FunctionWrapper <void(params...)>*>(this);
    }

};

#endif