#pragma once
#include "config.hpp"

#include <windows.h>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "skin.hpp"


class imgui_gui_class
{
public:
	imgui_gui_class() noexcept
	{
		//����imgui������
		ImGui::CreateContext();

		//����Ϸ����
		ImGui_ImplWin32_Init(FindWindowW(L"Valve001", NULL));

		//����imguiΪ��ɫ
		ImGui::StyleColorsLight();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;//����Ҫ����imgui�����ļ�
		io.LogFilename = nullptr;//����Ҫ����imgui��Ϣ
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;//����Ҫ�ı������ʽ

		//���������ļ�
		io.Fonts->AddFontFromFileTTF("c:\\msyh.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	}

	//��Ⱦ���溯��
	void render() noexcept 
	{
		if (!g_config.control.show_imgui) return;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(u8"CSGO��Ϸ����", &g_config.control.show_imgui);

		ImGui::Checkbox(u8"�Թ�����", &g_config.control.glow);
		ImGui::Checkbox(u8"������Ƥ��", &g_config.control.skin);

		static int weapon_kit_index_last = 0;//��һ�ε�����Ƥ��
		static int weapon_kit_index = 0;//��һ�ε�����Ƥ��

		ImGui::Combo(u8"����Ƥ��ѡ��", &weapon_kit_index, [](void* data, int idx, const char** out_text)
		{
			//����Ƥ���ַ���
			*out_text = g_config.control.skin_vector[idx].name.c_str();

			//��������Ƥ��ID
			g_config.control.weapon_skin_id = g_config.control.skin_vector[idx].id;
			return true;
		}, nullptr, g_config.control.skin_vector.size(), 10);

		//��һ������һ�β�һ������������ѡ��������Ƥ��
		if (weapon_kit_index_last != weapon_kit_index)
		{
			skin_space::schedule_hud_update();
			weapon_kit_index_last = weapon_kit_index;
		}

		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	//���ı�ת��Ϊutf8ʹ��imgui��������ʾ����
	std::string string_to_utf8(const std::string& str)
	{
		int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);
		WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string ret = pBuf;
		delete[]pwBuf;
		delete[]pBuf;

		return ret;
	}

	//��utf8����ת��Ϊ�ַ���
	std::string utf8_to_string(const std::string& str)
	{
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);
		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string ret = pBuf;
		delete[]pBuf;
		delete[]pwBuf;

		return ret;
	}
};

extern imgui_gui_class g_imgui;