#include "config.hpp"
#include "entity.h"
#include "sight_trace.hpp"

bool entity_class::is_enemy() noexcept
{
	return g_config.memory.is_other_enemy(this, g_config.entity_list->get_entity(g_config.engine->get_local_player()));
}

bool entity_class::is_visiable(const self_vector_struct& position) noexcept
{
	static sight_trace_struct sight_trace;

	//获取自己
	auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
	if (!local_player) return false;


	return true;
}
