#pragma once
#include <stdint.h>

//哈希获取结构
typedef struct fnv_struct
{
#define offsetBasis  0x811c9dc5
#define prime 0x1000193

	static uint32_t hash(const char* str, const uint32_t value = offsetBasis) noexcept
	{
		return *str ? hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
	}

	static uint32_t hash_runtime(const char* str) noexcept
	{
		auto value = offsetBasis;

		while (*str)
		{
			value ^= *str++;
			value *= prime;
		}
		return value;
	}
}fnv_struct;
