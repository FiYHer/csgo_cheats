#pragma once
#include <cstddef>
#include "user_cmd.hpp"

//ȫ�ֵ��ڴ������Ϣ
typedef struct global_vars_struct 
{
	const float real_time;//ʵʱʱ��
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

