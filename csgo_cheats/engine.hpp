#pragma once
#include "help_func.hpp"
#include "player_info.hpp"
#include "self_vector.hpp"

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

	//��ȡȫ����Ϸ�������
	constexpr auto get_max_clients() noexcept
	{
		return call_virtual_method<int>(this, 20);
	}

	//��ȡ�������
	using self_matrix = float[4][4];
	constexpr auto world_to_screen_matrix() noexcept
	{
		return call_virtual_method<const self_matrix&>(this, 37);
	}

	//��ȡ����Ƕ�
	constexpr auto get_view_angles(self_vector_struct& angles) noexcept
	{
		call_virtual_method<void, self_vector_struct&>(this, 18, angles);
	}

	//��������Ƕ�
	constexpr auto set_view_angles(const self_vector_struct& angles) noexcept
	{
		call_virtual_method<void, const self_vector_struct&>(this, 19, angles);
	}


};