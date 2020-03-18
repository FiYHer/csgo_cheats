#pragma once
#include "self_vector.hpp"

typedef struct ray_struct 
{
	ray_struct(const self_vector_struct& src, const self_vector_struct& dest) :
		start(src),
		delta(dest - src) 
	{
		is_swept = delta.x || delta.y || delta.z;
	}

	self_vector_struct start{ };
	float pad{ };
	self_vector_struct delta{ };
	std::byte pad2[40]{ };
	bool is_ray{ true };
	bool is_swept{ };
}ray_struct;