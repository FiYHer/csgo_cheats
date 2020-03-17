#pragma once
#include <cstddef>
#include "user_cmd.hpp"

//全局的内存变量信息
typedef struct global_vars_struct 
{
	const float real_time;//实时时间
	const int frame_count;
	const float absolute_frame_time;
	const std::byte pad[4];
	float current_time;
	float frame_time;
	const int max_clients;
	const int tick_count;
	const float interval_per_tick;

	float serverTime(user_cmd_struct* cmd = nullptr) noexcept;
}global_vars_struct;

