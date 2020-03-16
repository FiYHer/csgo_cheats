#pragma once
#include "help_func.hpp"

//
class localize_class
{
public:
	//
	constexpr auto find(const char* tokenName) noexcept
	{
		return call_virtual_method<wchar_t*, const char*>(this, 12, tokenName);
	}
};