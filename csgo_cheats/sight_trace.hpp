#pragma once
#include "self_vector.hpp"
#include "entity.h"

//玩家视线跟踪
typedef struct sight_trace_struct
{
	self_vector_struct start_pos;//开始位置
	self_vector_struct end_pos;//结束位置
	std::byte pad[20];//未知数据
	float fraction;
	int contents;
	unsigned short disp_flags;
	bool all_solid;
	bool start_solid;
	std::byte pad1[4];
	struct _surface_struct
	{
		const char* name;
		short surface_props;
		unsigned short flags;
	} surface_struct;
	int hit_group;
	std::byte pad2[4];
	entity_class* entity;
	int hit_box;
}sight_trace_struct;