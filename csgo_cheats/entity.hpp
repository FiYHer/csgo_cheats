#pragma once
#include "help_func.hpp"
#include "client_class.hpp"
#include "net_vars.h"

#include <string>


//实例类
class entity_class
{
public:
	//判断该实例是否是处于休眠状态
	constexpr bool is_dormant() noexcept
	{
		return call_virtual_method<bool>(this + 8, 9);
	}

	//获取该实例的客户数据类
	constexpr client_class_struct* get_client_class() noexcept
	{
		return call_virtual_method<client_class_struct*>(this + 8, 2);
	}

public:

	NETVAR(get_health, "CBasePlayer", "m_iHealth", int)//获取玩家血量


};