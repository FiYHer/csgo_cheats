#pragma once
#include "help_func.hpp"

//��Ϸ�¼���
class game_event_class
{
public:
	//��ȡ��������
	constexpr auto get_int(const char* keyName) noexcept
	{
		return call_virtual_method<int, const char*, int>(this, 6, keyName, 0);
	}

	//��ȡ�ַ�������
	constexpr auto get_string(const char* keyName) noexcept
	{
		return call_virtual_method<const char*, const char*, const char*>(this, 9, keyName, "");
	}

	//�����ַ�������
	constexpr auto set_string(const char* keyName, const char* value) noexcept
	{
		call_virtual_method<void, const char*, const char*>(this, 16, keyName, value);
	}
};