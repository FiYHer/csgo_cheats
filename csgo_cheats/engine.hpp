#pragma once
#include "help_func.hpp"

//��Ϸ������
class engine_class
{
public:
	//��ȡ���������Ϣ
	constexpr auto get_local_player() noexcept
	{
		return call_virtual_method<int>(this, 12);
	}

	//�ж��Ƿ�����Ϸ��
	constexpr auto is_in_game() noexcept
	{
		return call_virtual_method<bool>(this, 26);
	}
};