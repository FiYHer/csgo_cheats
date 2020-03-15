#pragma once
#include "help_func.hpp"
#include "client_class.hpp"
#include "net_vars.h"

#include <string>


//ʵ����
class entity_class
{
public:
	//�жϸ�ʵ���Ƿ��Ǵ�������״̬
	constexpr bool is_dormant() noexcept
	{
		return call_virtual_method<bool>(this + 8, 9);
	}

	//��ȡ��ʵ���Ŀͻ�������
	constexpr client_class_struct* get_client_class() noexcept
	{
		return call_virtual_method<client_class_struct*>(this + 8, 2);
	}

public:

	NETVAR(get_health, "CBasePlayer", "m_iHealth", int)//��ȡ���Ѫ��


};