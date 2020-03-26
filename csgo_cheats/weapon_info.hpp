#pragma once

#include "weapon_item_definition_index.hpp"

// ������Ϣ
typedef struct weapon_info_struct
{
	constexpr weapon_info_struct(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon) {}

	const char* model;//ģ���ַ���
	const char* icon;//ͼ���ַ���
}weapon_info_struct;