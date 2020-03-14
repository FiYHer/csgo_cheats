#pragma once
#include "utl_vector.hpp"
#include "glow_object_definition.hpp"

//�Թ����������ṹ
typedef struct glow_object_mamager_struct
{
	//�Թⶨ���б�
	utl_vector_class<glow_object_definition_struct> glow_object_definitions;

	int firstFreeSlot;

	//ע���Թ����
	constexpr void un_register_glow_object(int index) noexcept
	{
		glow_object_definitions[index].nextFreeSlot = firstFreeSlot;
		glow_object_definitions[index].entity = nullptr;
		glow_object_definitions[index].renderWhenOccluded = false;
		glow_object_definitions[index].renderWhenUnoccluded = false;
		firstFreeSlot = index;
	}

}glow_object_mamager_struct;