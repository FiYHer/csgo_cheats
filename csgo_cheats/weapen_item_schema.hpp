#pragma once
#include <cstddef>
#include "head.hpp"
#include "paint_kit.hpp"

//ÎäÆ÷Æ¤·ôÏîÄ¿
class weapen_item_schema_class
{
private:
	void* vmt;
	std::byte pad[0x28C];
public:
	head_struct<int, paint_kit_struct*> paintKits;
};


