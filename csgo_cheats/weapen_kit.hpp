#pragma once
#include <string>
 
//ÎäÆ÷Æ¤·ô½á¹¹
typedef struct weapen_kit_struct
{
	weapen_kit_struct(int id, std::string&& name) noexcept : id(id), name(name) { }
	int id;
	std::string name;

	auto operator<(const weapen_kit_struct& other) const noexcept
	{
		return name < other.name;
	}
}weapen_kit_struct;