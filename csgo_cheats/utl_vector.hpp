#pragma once

//������������
template <typename T>
class utl_vector_class
{
public:
	constexpr T& operator[](int i) noexcept { return memory[i]; };

	T* memory;//����
	int allocationCount;//�Ѿ�����ռ������
	int growSize;//������С
	int size;//��ǰ��С
	T* elements;//Ԫ��
};