#pragma once


//�����麯����������
template<typename T, typename ...Args>
constexpr auto call_virtual_method(void* classBase, int index, Args... args) noexcept
{
	return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[index])(classBase, args...);
}