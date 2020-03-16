#pragma once
#include <type_traits>
#include "help_func.hpp"
#include "utl_vector.hpp"

//内存结构数据
typedef struct con_var_struct
{
	//获取浮点型数据
	constexpr float get_float() noexcept
	{
		return call_virtual_method<float>(this, 12);
	}

	//获取整型数据
	constexpr int get_int() noexcept
	{
		return call_virtual_method<int>(this, 13);
	}

	//设置字符串取值
	constexpr void set_value(const char* value) noexcept
	{
		call_virtual_method<void, const char*>(this, 14, value);
	}

	//设置浮点数值
	constexpr void set_value(float value) noexcept
	{
		call_virtual_method<void, float>(this, 15, value);
	}

	//设置整型值
	constexpr void set_value(int value) noexcept
	{
		call_virtual_method<void, int>(this, 16, value);
	}

	std::byte pad[24];//未知数据
	std::add_pointer_t<void()> changeCallback;//改变皮肤回调函数
	con_var_struct* parent;//父指针
	const char* defaultValue;//默认值
	char* string;//字符串名称
	std::byte pad1[28];//未知数据
	utl_vector_class<void()> onChangeCallbacks;//改变皮肤时的回调函数
}con_var_struct;
