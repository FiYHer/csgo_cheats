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

//�ͻ�ģʽ��
class client_mode_class;

//��Ϸ���׿��ƽṹ
typedef struct cheat_control_struct
{
	bool show_imgui;//��ʾimgui�˵�

	bool glow;//�Թ���ʾ����
	float glow_color[3];//�Թ���ɫ

	cheat_control_struct()
	{
		show_imgui = true;
	}
}cheat_control_struct;

//�ڴ���ؽṹ
typedef struct memory_struct
{
	uintptr_t present;//present������ַ
	uintptr_t reset;//reset������ַ

	

	glow_object_mamager_struct* glow_object_mamager;//�Թ�������ṹָ��

	memory_struct()
	{
		present = find_pattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
		reset = find_pattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
		glow_object_mamager = *reinterpret_cast<glow_object_mamager_struct**>(find_pattern(L"client_panorama", "\x0F\x11\x05????\x83\xC8\x01", 3));
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

	cheat_control_struct control;//���׿��ƽṹ
	memory_struct memory;//�ڴ���ؽṹ

	configuration_struct()
	{
		engine = find<engine_class>(L"engine", "VEngineClient014");
		entity_list = find<entity_list_class>(L"client_panorama", "VClientEntityList003");
		client = find<client_class>(L"client_panorama", "VClient018");
		client_mode = **reinterpret_cast<client_mode_class***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
		gmae_ui = find<game_ui_class>(L"client_panorama", "GameUI011");
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

extern configuration_struct g_config;


