#pragma once

//响向量辅助类
template <typename T>
class utl_vector_class
{
public:
	constexpr T& operator[](int i) noexcept { return memory[i]; };

	T* memory;//数据
	int allocationCount;//已经申请空间的数量
	int growSize;//增量大小
	int size;//当前大小
	T* elements;//元素
};