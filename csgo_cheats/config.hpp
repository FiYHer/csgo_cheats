#pragma once
#include <windows.h>
#include <vadefs.h>
#include <Psapi.h>

#include "engine.hpp"
#include "entity_list.hpp"
#include "glow_object_manager.hpp"
#include "client.hpp"
#include "game_ui.hpp"
#include "weapen_item_schema.hpp"
#include "localize.hpp"
#include "weapen_kit.hpp"
#include "cvar.hpp"
#include "user_cmd.hpp"
#include "global_vars.h"
#include "engine_sight_trace.hpp"
#include "model_info.hpp"

//�ͻ�ģʽ��
class client_mode_class;

//��Ϸ���׿��ƽṹ
typedef struct cheat_control_struct
{
	bool show_imgui = true;//��ʾimgui�˵�
	int language_english = 0;//ѡ��Ӣ��

	bool show_glow = false;//�Թ���ʾ����
	bool glow_enable = false;//�����Թ�
	bool glow_enemy = false;//�Թ����
	bool glow_friend = false;//�Թ����

	bool show_skin = false;//��ʾ��������
	bool skin_enable = false;//��������
	std::vector<weapen_kit_struct> skin_vector;//����Ƥ���б�
	int weapon_skin_id = 0;//����Ƥ��ID
	std::unordered_map<int,const char*> weapon_map;//ͨ��ID����ǹе
	int weapon_id = 0;//��ǰ����ID

	std::vector<weapen_kit_struct> knife_vector;//С��ģ���б�
	int knife_index = 0;//С������

	bool aim_show = false;//��ʾ���鴰��
	bool aim_enable = false;//��������
	int aim_bone = 8;//���鲿λ
	bool aim_auto_shot = false;//�Զ���ǹ
	bool aim_auto_scope = false;//�Զ�����
	bool aim_aimlock = false;//һֱ��������
	float aim_fov = 255.0f;//��������ķ�Χ
	float aim_max_aim_inaccuracy = 1.0f;//����������ȷ��
	float aim_max_shot_inaccuracy = 1.0f;//����������ȷ��
	
	bool show_report = false;//��ʾ�������
	bool report_enable =  false;//�����ٱ�
	int report_mode = 1;//�ٱ�ģʽ
	int report_interval = 5;//�ٱ����
	std::vector<player_info_struct> report_players;//��Ϸ���������Ϣ�б�
	uint64_t report_player_xuid;//�ٱ����xuid
	bool report_text_abuse = false;//����
	bool report_grief = false;//ɧ��
	bool report_wall_hack = true;//͸��
	bool report_aim_bot = true;//����
	bool report_speed_hack = false;//����

	bool show_other = false;//�������ܿ���
	bool other_again_jump = false;//��������

}cheat_control_struct;

//�ڴ���ؽṹ
typedef struct memory_struct
{
	uintptr_t present;//present������ַ
	uintptr_t reset;//reset������ַ

	glow_object_mamager_struct* glow_object_mamager;//�Թ�������ṹָ��

	std::add_pointer_t<weapen_item_schema_class* __cdecl()> weapen_item_schema;//����Ƥ��

	uintptr_t hud;//ͼ���ַ
	int*(__thiscall* find_hud_element)(uintptr_t, const char*);//����ͼ��Ԫ��
	int(__thiscall* clear_hud_weapon)(int*, int);//�������ͼ��

	bool(__thiscall* is_other_enemy)(entity_class*, entity_class*);//�ж��Ƿ��ǵ��˺���

	std::add_pointer_t<bool __stdcall(const char*, const char*)> submit_report;//�ٱ���Һ�����ַ

	memory_struct()
	{
		present = find_pattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
		reset = find_pattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
		glow_object_mamager = *reinterpret_cast<glow_object_mamager_struct**>(find_pattern(L"client_panorama", "\x0F\x11\x05????\x83\xC8\x01", 3));
		weapen_item_schema = relativeToAbsolute<decltype(weapen_item_schema)>(reinterpret_cast<int*>(find_pattern(L"client_panorama", "\xE8????\x0F\xB7\x0F", 1)));
		auto temp = reinterpret_cast<std::uintptr_t*>(find_pattern(L"client_panorama", "\xB9????\xE8????\x8B\x5D\x08", 1));
		hud = *temp;
		find_hud_element = relativeToAbsolute<decltype(find_hud_element)>(reinterpret_cast<int*>(reinterpret_cast<char*>(temp) + 5));
		clear_hud_weapon = reinterpret_cast<decltype(clear_hud_weapon)>(find_pattern(L"client_panorama", "\x55\x8B\xEC\x51\x53\x56\x8B\x75\x08\x8B\xD9\x57\x6B\xFE\x2C"));
		is_other_enemy = relativeToAbsolute<decltype(is_other_enemy)>(reinterpret_cast<int*>(find_pattern(L"client_panorama", "\xE8????\x02\xC0", 1)));
		submit_report = reinterpret_cast<decltype(submit_report)>(find_pattern(L"client_panorama", "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x28\x8B\x4D\x08"));

	}

	//�ڴ�ģʽ����
	uintptr_t find_pattern(const wchar_t* module, const char* pattern, size_t offset = 0) noexcept
	{
		//��ȡָ��ģ��������Ϣ
		MODULEINFO moduleInfo;
		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(module), &moduleInfo, sizeof(moduleInfo))) 
		{
			//��ȡ��ʼ��ַ�ͽ�����ַ
			auto start = static_cast<const char*>(moduleInfo.lpBaseOfDll);
			const auto end = start + moduleInfo.SizeOfImage;

			auto first = start;
			auto second = pattern;

			while (first < end && *second) 
			{
				if (*first == *second || *second == '?')
				{
					++first;
					++second;
				}
				else 
				{
					first = ++start;
					second = pattern;
				}
			}

			//���ҵ���ַ
			if (!*second) return reinterpret_cast<uintptr_t>(const_cast<char*>(start) + offset);
		}
		MessageBoxA(NULL, "findPattern", NULL, MB_OK | MB_ICONWARNING);
		exit(-1);
	}

}memory_struct;

//��Ϸ�������ýṹ
typedef struct configuration_struct
{
	engine_class* engine;//��Ϸ������ָ��
	entity_list_class* entity_list;//ʵ���б���ָ��
	client_class* client;//�ͻ�����ָ��
	client_mode_class* client_mode;//�ͻ�ģʽ��ָ��
	game_ui_class* gmae_ui;//��Ϸui��ָ��
	localize_class* localize;//��λ��ָ��
	cvar_class* cvar;//��ֵ��ָ��
	engine_trace_class* engine_trace;//�������߸�����ָ��
	model_info_class* model_info;//ģ����Ϣ��ָ��

	cheat_control_struct control;//���׿��ƽṹ
	memory_struct memory;//�ڴ���ؽṹ

	global_vars_struct* global_vars;//ȫ���ڴ�����ṹָ��

	configuration_struct()
	{
		engine = find<engine_class>(L"engine", "VEngineClient014");
		entity_list = find<entity_list_class>(L"client_panorama", "VClientEntityList003");
		client = find<client_class>(L"client_panorama", "VClient018");
		client_mode = **reinterpret_cast<client_mode_class***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
		gmae_ui = find<game_ui_class>(L"client_panorama", "GameUI011");
		localize = find<localize_class>(L"localize", "Localize_001");
		cvar = find<cvar_class>(L"vstdlib", "VEngineCvar007");
		global_vars = **reinterpret_cast<global_vars_struct***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);
		engine_trace = find<engine_trace_class>(L"engine", "EngineTraceClient004");
		model_info = find<model_info_class>(L"engine", "VModelInfoClient004");

	}

	//����ָ���Ľӿ�
	template <typename T>
	auto find(const wchar_t* module, const char* name) noexcept
	{
		if (HMODULE moduleHandle = GetModuleHandleW(module))
			if (const auto createInterface = reinterpret_cast<std::add_pointer_t<T* (const char* name, int* returnCode)>>(GetProcAddress(moduleHandle, "CreateInterface")))
				if (T* foundInterface = createInterface(name, nullptr))
					return foundInterface;

		MessageBoxA(nullptr, "find", NULL, MB_OK | MB_ICONERROR);
		std::exit(EXIT_FAILURE);
	}

}configuration_struct;

//�������ⲿ��
extern configuration_struct g_config;


