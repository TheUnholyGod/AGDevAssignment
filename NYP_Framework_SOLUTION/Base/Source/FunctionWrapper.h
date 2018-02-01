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
	virtual ~IFunction() {};
    virtual void Invoke() = 0;
    std::string GetID() { return m_ID; }
};

template<typename ret> 
class FunctionWrapper : public IFunction
{
private:
	typedef std::function<ret()> FunctionPointer;
	FunctionPointer m_function;
public:
    FunctionWrapper <ret>(std::string _ID) :IFunction(_ID) {}
	FunctionWrapper <ret>(FunctionPointer _function) : m_function(_function) {}

	~FunctionWrapper() {}

    void Invoke()
	{
		m_function();
	}
};

template<typename ret, typename... params>
class FunctionWrapper <ret(params...)>: public IFunction
{
private:
	typedef std::function<ret(params...)> FunctionPointer;
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

    void Invoke()
	{
		typedef typename std::decay<Parameters>::type ttype;
		Unpacker<0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(m_function, std::forward<Parameters>(m_parameterpack));
	}

    ret Invoke(params... _params)
	{
		return m_function(_params...);
	}

};

#endif