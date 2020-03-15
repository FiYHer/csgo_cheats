#pragma once
#include <unordered_map>
#include "rect_table.hpp"
#include "fnv.hpp"

//�������������
class net_vars_class
{
public:
	//��ʼ��
	net_vars_class() noexcept;

	//�ָ�
	void restore() noexcept;

	//��������
	uint16_t operator[](const uint32_t hash) noexcept;
private:
	//��ȡƫ������
	void walk_table(bool unload, const char* networkName, recv_table_struct* recvTable, const std::size_t  offset = 0) noexcept;

	//ƫ���б�
	std::unordered_map<uint32_t, uint16_t> offsets;
};

extern net_vars_class g_net_vars;

#define NETVAR_OFFSET(funcname, class_name, var_name, offset, type) \
std::add_lvalue_reference_t<type> funcname() noexcept \
{ \
    auto hash{ fnv_struct::hash(class_name "->" var_name) }; \
    return *reinterpret_cast<std::add_pointer_t<type>>(this + g_net_vars[hash] + offset); \
}

#define NETVAR(funcname, class_name, var_name, type) \
    NETVAR_OFFSET(funcname, class_name, var_name, 0, type)

