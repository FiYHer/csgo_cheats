#pragma once
#include "node.hpp"

//����Ƥ�����ݴ�Ÿ���ͷ�ṹ
template <typename Key, typename Value>
struct head_struct
{
	node_struct<Key, Value>* memory;//����
	int allocationCount;//�����˵Ĵ�С
	int growSize;//������С
	int startElement;//��ʼԪ��
	int nextAvailable;//��һ��
	int _unknown;//δ֪
	int lastElement;//��һ��Ԫ��
};
