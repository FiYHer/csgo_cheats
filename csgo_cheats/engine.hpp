#pragma once
#include "help_func.hpp"
#include "player_info.hpp"

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

	//��ȡָ��������������Ϣ
	constexpr auto get_player_info(int entityIndex, const player_info_struct& playerInfo) noexcept
	{
		return call_virtual_method<bool, int, const player_info_struct&>(this, 8, entityIndex, playerInfo);
	}

	//�����û�ID��ȡ���
	constexpr auto get_player_for_user_id(int userId) noexcept
	{
		return call_virtual_method<int, int>(this, 9, userId);
	}

};