#pragma once

//�����ַ����ṹ
typedef struct self_string_struct
{
	char* buffer;//����
	int capacity;//�����˴�С
	int growSize;//������С
	int length;//����
}self_string_struct;