#pragma once
#include "help_func.hpp"

//游戏ui消息显示类
class game_ui_class
{
public:
	//显示游戏窗口消息
	constexpr void message_box(const char* title, const char* text) noexcept
	{
		call_virtual_method<void, const char*, const char*, bool, bool, const char*, const char*, const char*, const char*, void*>(this, 20, title, text, true, false, nullptr, nullptr, nullptr, nullptr, nullptr);
	}
};