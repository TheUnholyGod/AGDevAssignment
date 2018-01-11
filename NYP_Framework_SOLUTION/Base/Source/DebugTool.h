#ifndef _DEBUG_TOOL_H
#define _DEBUG_TOOL_H

#include "SingletonTemplate.h"

#include <map>
#include <functional>

class DebugTool : public Singleton<DebugTool>
{
	friend Singleton<DebugTool>;
private:
	std::map<char, std::function<void()>> m_keybindings;
public:
	template<typename Lambda>
	void BindKey(char _key, Lambda&& func)
	{
		if (m_keybindings.find(_key) != m_keybindings.end())
			return;
		m_keybindings[_key] = func;
	}
	void Init();
	void Update(double _dt);
	void Run(char _key);
	void Exit();
};

#endif