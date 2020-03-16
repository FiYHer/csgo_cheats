#pragma once

//节点模板结构
template <typename Key, typename Value>
struct node_struct
{
	int previousId;//上一个id
	int nextId;//下一个id
	void* _unknownPtr;//未知指针
	int _unknown;//未知值
	Key key;
	Value value;
};