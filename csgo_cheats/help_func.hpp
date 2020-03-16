#pragma once

//�����麯����������
template<typename T, typename ...Args>
constexpr auto call_virtual_method(void* classBase, int index, Args... args) noexcept
{
	return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[index])(classBase, args...);
}

//��Ե�ַת��Ϊ���Ե�ַ
template <typename T>
constexpr auto relativeToAbsolute(int* address) noexcept
{
	return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}