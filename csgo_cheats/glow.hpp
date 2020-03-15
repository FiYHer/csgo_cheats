#pragma once
#include "config.hpp"

#include <vector>

//�Թ���������ռ�
namespace glow_space
{
	//�Զ���Թ���Ϣ
	std::vector<std::pair<int, int>> custom_glow_entities;

	//����Զ���Թ����
	void clear_custom_objects() noexcept
	{
		for (const auto& [entity_index, glow_object_index] : custom_glow_entities)
			g_config.memory.glow_object_mamager->un_register_glow_object(glow_object_index);
		custom_glow_entities.clear();
	}

	//���лԹ����
	void render() noexcept
	{
		//û����������Թ�
		if (!g_config.control.glow) return;

		//��ȡ������Ϣ
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player) return;

		//��������Զ���Թ����
		clear_custom_objects();

		//�ԻԹ������в���
		for (int i = 0; i < g_config.memory.glow_object_mamager->glow_object_definitions.size; i++)
		{
			//��ȡ�Թ������Ϣ
			glow_object_definition_struct& glow_object = g_config.memory.glow_object_mamager->glow_object_definitions[i];

			//��ȡ�ûԹ�����Ӧ��ʵ��
			auto entity = glow_object.entity;
			if(glow_object.is_unused() || !entity || entity->is_dormant()) continue;

			//��������
			if (entity->get_client_class()->classId == classId_enum::CSPlayer)
			{
				glow_object.renderWhenOccluded = true;//����Ϊ���������Ҳ�Թ�
				glow_object.alpha = 1.0f;//һ��Ҳ��͸��
				glow_object.glowStyle = 0;//Ĭ������
				glow_object.bloomAmount = 1.0f;//���
				int health = local_player->get_health();//��ȡ����Ѫ��
				glow_object.glowColor = { 1.0f - health / 100.0f,  health / 100.0f, 0.0f };//��������
			}
		}
	}
}