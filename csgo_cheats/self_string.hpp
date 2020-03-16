#pragma once

//辅助字符串结构
typedef struct self_string_struct
{
	char* buffer;//内容
	int capacity;//申请了大小
	int growSize;//增长大小
	int length;//长度
}self_string_struct;