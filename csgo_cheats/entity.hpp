#pragma once
#include "help_func.hpp"
#include "client_class.hpp"
#include "net_vars.h"

#include <string>


//ʵ����
class entity_class
{
public:
	//�жϸ�ʵ���Ƿ��Ǵ�������״̬
	constexpr bool is_dormant() noexcept
	{
		return call_virtual_method<bool>(this + 8, 9);
	}

	//��ȡ��ʵ���Ŀͻ�������
	constexpr client_class_struct* get_client_class() noexcept
	{
		return call_virtual_method<client_class_struct*>(this + 8, 2);
	}

	//�ж�����Ƿ�����
	constexpr bool is_alive() noexcept
	{
		return call_virtual_method<bool>(this, 155) && get_health() > 0;
	}

	//���ݾ����ȡ
	constexpr auto get_entity_from_handle(int handle) noexcept
	{
		return call_virtual_method<entity_class*, int>(this, 4, handle);
	}

public:

	NETVAR(get_health, "CBasePlayer", "m_iHealth", int)//��ȡ���Ѫ��
	NETVAR_OFFSET(get_index, "CBaseEntity", "m_bIsAutoaimTarget", 4, int)//��ȡ����
	NETVAR(get_weapons, "CBaseCombatCharacter", "m_hMyWeapons", int[48])//������Ϣ
	NETVAR(get_item_definition_index, "CBaseAttributableItem", "m_iItemDefinitionIndex", short)//��ȡ������������
	NETVAR(get_item_id_high, "CBaseAttributableItem", "m_iItemIDHigh", int)//
	NETVAR(get_account_id, "CBaseAttributableItem", "m_iAccountID", int)//
	NETVAR(get_fallback_paint_kit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned)//
	NETVAR(get_view_model, "CBasePlayer", "m_hViewModel[0]", int)//
	NETVAR(get_weapon, "CBaseViewModel", "m_hWeapon", int)//
	NETVAR(get_entity_quality, "CBaseAttributableItem", "m_iEntityQuality", int)//
	NETVAR(get_fallback_seed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned)//
	NETVAR(get_fallback_stat_trak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned)//
	NETVAR(get_fallback_wear, "CBaseAttributableItem", "m_flFallbackWear", float)//


};