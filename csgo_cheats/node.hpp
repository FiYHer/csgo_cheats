#pragma once

//�ڵ�ģ��ṹ
template <typename Key, typename Value>
struct node_struct
{
	int previousId;//��һ��id
	int nextId;//��һ��id
	void* _unknownPtr;//δָ֪��
	int _unknown;//δֵ֪
	Key key;
	Value value;
};