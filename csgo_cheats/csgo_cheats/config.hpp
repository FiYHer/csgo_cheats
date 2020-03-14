#pragma once

#include <windows.h>
#include <vadefs.h>
#include <Psapi.h>

//ǰ������
class engine_class;

//�ڴ���ؽṹ
typedef struct memory_struct
{
	uintptr_t present;//present������ַ
	uintptr_t reset;//reset������ַ

	memory_struct()
	{
		present = find_pattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB", 2);
		reset = find_pattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8", 9);
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



	memory_struct memory;//�ڴ���ؽṹ

	configuration_struct()
	{

	}
}configuration_struct;

extern configuration_struct g_config;


