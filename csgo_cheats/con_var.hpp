#pragma once
#include <type_traits>
#include "help_func.hpp"
#include "utl_vector.hpp"

typedef struct con_var_struct
{
	//获取浮点型数据
	constexpr float getFloat() noexcept
	{
		return call_virtual_method<float>(this, 12);
	}

	//获取整型数据
	constexpr int getInt() noexcept
	{
		return call_virtual_method<int>(this, 13);
	}

	//设置字符串取值
	constexpr void setValue(const char* value) noexcept
	{
		call_virtual_method<void, const char*>(this, 14, value);
	}

	//设置浮点数值
	constexpr void setValue(float value) noexcept
	{
		call_virtual_method<void, float>(this, 15, value);
	}

	//设置整型值
	constexpr void setValue(int value) noexcept
	{
		call_virtual_method<void, int>(this, 16, value);
	}

	std::byte pad[24];
	std::add_pointer_t<void()> changeCallback;//改变皮肤回调函数
	con_var_struct* parent;
	const char* defaultValue;
	char* string;
	std::byte pad1[28];
	utl_vector_class<void()> onChangeCallbacks;
}con_var_struct;
