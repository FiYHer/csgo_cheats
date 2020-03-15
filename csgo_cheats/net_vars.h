#pragma once
#include <unordered_map>
#include "rect_table.hpp"
#include "fnv.hpp"

//网络变量辅助类
class net_vars_class
{
public:
	//初始化
	net_vars_class() noexcept;

	//恢复
	void restore() noexcept;

	//重载索引
	uint16_t operator[](const uint32_t hash) noexcept;
private:
	//获取偏移数据
	void walk_table(bool unload, const char* networkName, recv_table_struct* recvTable, const std::size_t  offset = 0) noexcept;

	//偏移列表
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

