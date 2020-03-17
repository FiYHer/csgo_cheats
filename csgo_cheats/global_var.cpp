#include "global_vars.h"
#include "config.hpp"

float global_vars_struct::serverTime(user_cmd_struct* cmd /*= nullptr*/) noexcept
{
	static int last_tick;
	static user_cmd_struct* last_cmd;

	if (cmd)
	{
		if (!last_cmd || last_cmd->has_been_predicted) last_tick = g_config.entity_list->get_entity(g_config.engine->get_local_player())->get_tick_base();
		else last_tick++;
		last_cmd = cmd;
	}

	return last_tick * interval_per_tick;
}
