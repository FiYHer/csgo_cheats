#pragma once
#include "node.hpp"

//武器皮肤数据存放辅助头结构
template <typename Key, typename Value>
struct head_struct
{
	node_struct<Key, Value>* memory;//内容
	int allocationCount;//申请了的大小
	int growSize;//增长大小
	int startElement;//开始元素
	int nextAvailable;//下一个
	int _unknown;//未知
	int lastElement;//上一个元素
};
