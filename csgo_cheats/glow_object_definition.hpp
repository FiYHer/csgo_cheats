#pragma once
#include "entity.h"
#include "self_vector.hpp"
#include <cstddef>

//辉光对象定义结构
typedef struct glow_object_definition_struct
{
	entity_class* entity;
	self_vector_struct glowColor;
	float   alpha;
	std::byte pad[4];
	float   m_flSomeFloat;
	float   bloomAmount;
	float   m_flAnotherFloat;
	bool    renderWhenOccluded;
	bool    renderWhenUnoccluded;
	bool    fullBloomRender;
	std::byte pad1;
	int fullBloomStencilTestValue;
	int glowStyle;
	int splitScreenSlot;
	int nextFreeSlot;

	constexpr bool is_unused() const noexcept { return nextFreeSlot != ENTRY_IN_USE; }

	static constexpr int END_OF_FREE_LIST = -1;
	static constexpr int ENTRY_IN_USE = -2;
}glow_object_definition_struct;