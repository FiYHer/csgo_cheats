#pragma once
#include "help_func.hpp"
#include "entity.h"

//实例列表类
class entity_list_class
{
public:
	//获取指定索引实例
	constexpr auto get_entity(int index) noexcept
	{
		return call_virtual_method<entity_class*, int>(this, 3, index);
	}

	//根据句柄获取实例
	constexpr auto get_entity_from_handle(int handle) noexcept
	{
		return call_virtual_method<entity_class*, int>(this, 4, handle);
	}
};