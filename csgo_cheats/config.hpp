#pragma once
#include "engine.hpp"
#include "entity.hpp"
#include "entity_list.hpp"
#include "glow_object_manager.hpp"

#include <windows.h>
#include <vadefs.h>
#include <Psapi.h>
#include "client.hpp"
#include "game_ui.hpp"

//客户模式类
class client_mode_class;

//游戏作弊控制结构
typedef struct cheat_control_struct
{
	bool show_imgui;//显示imgui菜单

	bool glow;//辉光显示控制
	float glow_color[3];//辉光颜色

	cheat_control_struct()
	{
		show_imgui = true;
	}
}cheat_control_struct;

//内存相关结构
typedef struct memory_struct
{
	uintptr_t present;//present函数地址
	uintptr_t reset;//reset函数地址

	

	glow_object_mamager_struct* glow_object_mamager;//辉光对象管理结构指针

	memory_struct()
	{
		present = find_pattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
		reset = find_pattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
		glow_object_mamager = *reinterpret_cast<glow_object_mamager_struct**>(find_pattern(L"client_panorama", "\x0F\x11\x05????\x83\xC8\x01", 3));
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

	cheat_control_struct control;//作弊控制结构
	memory_struct memory;//内存相关结构

	configuration_struct()
	{
		engine = find<engine_class>(L"engine", "VEngineClient014");
		entity_list = find<entity_list_class>(L"client_panorama", "VClientEntityList003");
		client = find<client_class>(L"client_panorama", "VClient018");
		client_mode = **reinterpret_cast<client_mode_class***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
		gmae_ui = find<game_ui_class>(L"client_panorama", "GameUI011");
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

extern configuration_struct g_config;


