#include "DebugTool.h"
#include "KeyboardController.h"

void DebugTool::Init()
{
}

void DebugTool::Update(double _dt)
{
	for (auto&i : m_keybindings)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(i.first))
			i.second();
	}
}

void DebugTool::Run(char _key)
{
	if (m_keybindings.find(_key) == m_keybindings.end())
		return;

	m_keybindings[_key]();
}
