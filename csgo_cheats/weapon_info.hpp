#pragma once

#include "weapon_item_definition_index.hpp"

// 武器信息
typedef struct weapon_info_struct
{
	constexpr weapon_info_struct(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon) {}

	const char* model;//模型字符串
	const char* icon;//图标字符串
}weapon_info_struct;