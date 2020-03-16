#pragma once
#include <type_traits>
#include "help_func.hpp"
#include "utl_vector.hpp"

typedef struct con_var_struct
{
	//��ȡ����������
	constexpr float getFloat() noexcept
	{
		return call_virtual_method<float>(this, 12);
	}

	//��ȡ��������
	constexpr int getInt() noexcept
	{
		return call_virtual_method<int>(this, 13);
	}

	//�����ַ���ȡֵ
	constexpr void setValue(const char* value) noexcept
	{
		call_virtual_method<void, const char*>(this, 14, value);
	}

	//���ø�����ֵ
	constexpr void setValue(float value) noexcept
	{
		call_virtual_method<void, float>(this, 15, value);
	}

	//��������ֵ
	constexpr void setValue(int value) noexcept
	{
		call_virtual_method<void, int>(this, 16, value);
	}

	std::byte pad[24];
	std::add_pointer_t<void()> changeCallback;//�ı�Ƥ���ص�����
	con_var_struct* parent;
	const char* defaultValue;
	char* string;
	std::byte pad1[28];
	utl_vector_class<void()> onChangeCallbacks;
}con_var_struct;
