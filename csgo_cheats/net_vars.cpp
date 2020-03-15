#include "net_vars.h"
#include "config.hpp"


net_vars_class::net_vars_class() noexcept
{
	for (auto clientClass = g_config.client->get_all_classes(); clientClass; clientClass = clientClass->next)
		walk_table(false, clientClass->network_name, clientClass->recv_table);
}

void net_vars_class::restore() noexcept
{

}

uint16_t net_vars_class::operator[](const uint32_t hash) noexcept
{
	return offsets[hash];
}

void net_vars_class::walk_table(bool unload, const char* networkName, recv_table_struct* recvTable, const std::size_t offset) noexcept
{
	for (int i = 0; i < recvTable->propCount; ++i)
	{
		//��ȡ����
		auto& prop = recvTable->props[i];

		//ʮ�����ж�
		if (isdigit(prop.name[0]))  continue;

		//��ϣ���ж�
		if (fnv_struct::hash_runtime(prop.name) == fnv_struct::hash("baseclass")) continue;

		//�ݹ�
		if (prop.type == 6
			&& prop.dataTable
			&& prop.dataTable->netTableName[0] == 'D')
			walk_table(unload, networkName, prop.dataTable, prop.offset + offset);

		//��ȡhash
		const auto hash{ fnv_struct::hash_runtime((networkName + std::string{ "->" } +prop.name).c_str()) };

		//û�м���
		if (!unload) offsets[hash] = uint16_t(offset + prop.offset);
	}
}

