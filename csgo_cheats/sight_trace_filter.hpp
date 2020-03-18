#pragma once
#include "entity.h"

typedef struct trace_filter_struct
{
	trace_filter_struct(const entity_class* entity) : skip{ entity } { }
	virtual bool should_hit_entity(entity_class* entity, int) { return entity != skip; }
	virtual int get_trace_type() const { return 0; }
	const void* skip;
}trace_filter_struct;