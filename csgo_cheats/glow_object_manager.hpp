#pragma once
#include "utl_vector.hpp"
#include "glow_object_definition.hpp"

//辉光对象管理器结构
typedef struct glow_object_mamager_struct
{
	//辉光定义列表
	utl_vector_class<glow_object_definition_struct> glow_object_definitions;

	int firstFreeSlot;

	//注销辉光对象
	constexpr void un_register_glow_object(int index) noexcept
	{
		glow_object_definitions[index].nextFreeSlot = firstFreeSlot;
		glow_object_definitions[index].entity = nullptr;
		glow_object_definitions[index].renderWhenOccluded = false;
		glow_object_definitions[index].renderWhenUnoccluded = false;
		firstFreeSlot = index;
	}

}glow_object_mamager_struct;