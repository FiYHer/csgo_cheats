#pragma once
#include "self_vector.hpp"

//�������ṹ
typedef struct user_cmd_struct
{
	enum 
	{
		IN_ATTACK = 1 << 0,//���
		IN_JUMP = 1 << 1,//��Ծ
		IN_DUCK = 1 << 2,//�¶�
		IN_FORWARD = 1 << 3,//ǰ��
		IN_BACK = 1 << 4,//����
		IN_USE = 1 << 5,
		IN_MOVELEFT = 1 << 9,//���ƶ�
		IN_MOVERIGHT = 1 << 10,//���ƶ�
		IN_ATTACK2 = 1 << 11,//�Ҽ�
		IN_SCORE = 1 << 16,//����
		IN_BULLRUSH = 1 << 22//��
	};
	int pad;
	int command_number;
	int tick_count;
	self_vector_struct view_angles;
	self_vector_struct aim_direction;
	float forward_move;
	float side_move;
	float up_move;
	int buttons;
	char impulse;
	int weapon_select;
	int weapon_sub_type;
	int random_seed;
	short mouse_dx;
	short mouse_dy;
	bool has_been_predicted;
}user_cmd_struct;
