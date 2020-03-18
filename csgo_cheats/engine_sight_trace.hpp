#pragma once
#include "ray.hpp"
#include "sight_trace_filter.hpp"
#include "sight_trace.hpp"

class engine_trace_class
{
public:
	constexpr auto get_point_contents(const self_vector_struct& absPosition, int contentsMask) noexcept
	{
		return call_virtual_method<int, const self_vector_struct&, int, entity_class*>(this, 0, absPosition, contentsMask, nullptr);
	}

	constexpr void trace_ray(const ray_struct& ray, unsigned int mask, const trace_filter_struct& filter, sight_trace_struct& trace) noexcept
	{
		call_virtual_method<void, const ray_struct&, unsigned int, const trace_filter_struct&, sight_trace_struct&>(this, 5, ray, mask, filter, trace);
	}
};
