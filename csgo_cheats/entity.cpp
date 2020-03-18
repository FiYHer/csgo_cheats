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

	//获取自己看敌人位置的状态
	g_config.engine_trace->trace_ray({ local_player->get_eye_position(),position }, 0x46004009, { local_player }, sight_trace);

	return sight_trace.entity == this || sight_trace.fraction > 0.97f;
}

self_vector_struct entity_class::get_eye_position() noexcept
{
	self_vector_struct ret;
	call_virtual_method<void, self_vector_struct&>(this, 284, ret);
	return ret;
}

self_vector_struct entity_class::get_bone_position(int bone) noexcept
{
	matrix3x4_class head_matrix[128];
	setup_bones(head_matrix, 128, 256, 0.0f);
	return self_vector_struct{ head_matrix[bone][0][3], head_matrix[bone][1][3], head_matrix[bone][2][3] };
}

constexpr bool entity_class::setup_bones(matrix3x4_class* out, int max_bones, int bone_mask, float current_time) noexcept
{
	return call_virtual_method<bool, matrix3x4_class*, int, int, float>(this + 4, 13, out, max_bones, bone_mask, current_time);
}
