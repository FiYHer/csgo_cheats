#pragma once

//调用虚函数辅助函数
template<typename T, typename ...Args>
constexpr auto call_virtual_method(void* classBase, int index, Args... args) noexcept
{
	return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[index])(classBase, args...);
}

//相对地址转化为绝对地址
template <typename T>
constexpr auto relativeToAbsolute(int* address) noexcept
{
	return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}