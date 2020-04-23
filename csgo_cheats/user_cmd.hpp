#pragma once
#include "self_vector.hpp"

//玩家命令结构
typedef struct user_cmd_struct
{
	enum 
	{
		IN_ATTACK = 1 << 0,//左键
		IN_JUMP = 1 << 1,//跳跃
		IN_DUCK = 1 << 2,//下蹲
		IN_FORWARD = 1 << 3,//前进
		IN_BACK = 1 << 4,//后退
		IN_USE = 1 << 5,
		IN_MOVELEFT = 1 << 9,//左移动
		IN_MOVERIGHT = 1 << 10,//右移动
		IN_ATTACK2 = 1 << 11,//右键
		IN_SCORE = 1 << 16,//开镜
		IN_BULLRUSH = 1 << 22//狂奔
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
