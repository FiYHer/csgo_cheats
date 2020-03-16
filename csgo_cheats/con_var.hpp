#pragma once
#include <type_traits>
#include "help_func.hpp"
#include "utl_vector.hpp"

//�ڴ�ṹ����
typedef struct con_var_struct
{
	//��ȡ����������
	constexpr float get_float() noexcept
	{
		return call_virtual_method<float>(this, 12);
	}

	//��ȡ��������
	constexpr int get_int() noexcept
	{
		return call_virtual_method<int>(this, 13);
	}

	//�����ַ���ȡֵ
	constexpr void set_value(const char* value) noexcept
	{
		call_virtual_method<void, const char*>(this, 14, value);
	}

	//���ø�����ֵ
	constexpr void set_value(float value) noexcept
	{
		call_virtual_method<void, float>(this, 15, value);
	}

	//��������ֵ
	constexpr void set_value(int value) noexcept
	{
		call_virtual_method<void, int>(this, 16, value);
	}

	std::byte pad[24];//δ֪����
	std::add_pointer_t<void()> changeCallback;//�ı�Ƥ���ص�����
	con_var_struct* parent;//��ָ��
	const char* defaultValue;//Ĭ��ֵ
	char* string;//�ַ�������
	std::byte pad1[28];//δ֪����
	utl_vector_class<void()> onChangeCallbacks;//�ı�Ƥ��ʱ�Ļص�����
}con_var_struct;
