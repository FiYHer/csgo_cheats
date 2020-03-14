#pragma once

#include <windows.h>
#include <vadefs.h>
#include <Psapi.h>

//前向声明
class engine_class;

//内存相关结构
typedef struct memory_struct
{
	uintptr_t present;//present函数地址
	uintptr_t reset;//reset函数地址

	memory_struct()
	{
		present = find_pattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
		reset = find_pattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
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



	memory_struct memory;//内存相关结构

	configuration_struct()
	{

	}
}configuration_struct;

extern configuration_struct g_config;


