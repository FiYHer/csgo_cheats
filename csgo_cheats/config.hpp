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

//客户模式类
class client_mode_class;

//游戏作弊控制结构
typedef struct cheat_control_struct
{
	bool show_imgui;//显示imgui菜单

	bool glow;//辉光显示控制
	bool glow_enemy;//辉光敌人
	bool glow_friend;//辉光队友

	bool skin;//换皮肤控制
	std::vector<weapen_kit_struct> skin_vector;//武器皮肤列表
	int weapon_skin_id;//武器皮肤ID
	std::unordered_map<int,const char*> weapon_map;//通过ID查找枪械
	int weapon_id;//当前武器ID

	std::vector<weapen_kit_struct> knife_vector;//小刀模型列表
	int knife_index;//小刀索引

	bool aim;//自瞄开关控制
	bool aim_scoped;//开镜自瞄
	bool aim_fire;//开枪自瞄
	bool aim_quiet_step;//静步自瞄
	bool aim_auto_scoped;//瞄准敌人后自动开镜
	bool aim_auto_fire;//瞄准敌人后自动开枪
	bool aim_close_scoped;//自瞄后关闭开镜状态
	float aim_offset;//自瞄微调
	float aim_max_angle;//在准星范围内的敌人才进行自瞄

	bool report;//举报开关控制
	int report_mode;//举报模式
	int report_interval;//举报间隔
	std::vector<player_info_struct> report_players;//游戏房间玩家信息列表
	uint64_t report_player_xuid;//举报玩家xuid
	bool report_text_abuse;//骂人
	bool report_grief;//骚扰
	bool report_wall_hack;//透视
	bool report_aim_bot;//自瞄
	bool report_speed_hack;//加速

	bool other;//其它功能开关
	bool other_again_jump;//连跳功能


	cheat_control_struct()
	{
		show_imgui = true;

		aim_offset = 1.5f;
		aim_max_angle = 20.0f;

		report_interval = 5;

		report_wall_hack = report_aim_bot = true;
	}
}cheat_control_struct;

//内存相关结构
typedef struct memory_struct
{
	uintptr_t present;//present函数地址
	uintptr_t reset;//reset函数地址

	glow_object_mamager_struct* glow_object_mamager;//辉光对象管理结构指针

	std::add_pointer_t<weapen_item_schema_class* __cdecl()> weapen_item_schema;//武器皮肤

	uintptr_t hud;//图标地址
	int*(__thiscall* find_hud_element)(uintptr_t, const char*);//查找图标元素
	int(__thiscall* clear_hud_weapon)(int*, int);//清空武器图标

	bool(__thiscall* is_other_enemy)(entity_class*, entity_class*);//判断是否是敌人函数

	std::add_pointer_t<bool __stdcall(const char*, const char*)> submit_report;//举报玩家函数地址

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

	//内存模式查找
	uintptr_t find_pattern(const wchar_t* module, const char* pattern, size_t offset = 0) noexcept
	{
		//获取指定模块的相关信息
		MODULEINFO moduleInfo;
		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(module), &moduleInfo, sizeof(moduleInfo))) 
		{
			//获取开始地址和结束地址
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

			//查找到地址
			if (!*second) return reinterpret_cast<uintptr_t>(const_cast<char*>(start) + offset);
		}
		MessageBoxA(NULL, "findPattern", NULL, MB_OK | MB_ICONWARNING);
		exit(-1);
	}

}memory_struct;

//游戏辅助配置结构
typedef struct configuration_struct
{
	engine_class* engine;//游戏引擎类指针
	entity_list_class* entity_list;//实例列表类指针
	client_class* client;//客户端类指针
	client_mode_class* client_mode;//客户模式类指针
	game_ui_class* gmae_ui;//游戏ui类指针
	localize_class* localize;//定位类指针
	cvar_class* cvar;//数值类指针
	engine_trace_class* engine_trace;//引擎视线跟踪类指针
	model_info_class* model_info;//模型信息类指针

	cheat_control_struct control;//作弊控制结构
	memory_struct memory;//内存相关结构

	global_vars_struct* global_vars;//全局内存变量结构指针

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

	//查找指定的接口
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

//导出给外部用
extern configuration_struct g_config;


