#pragma once
#include "config.hpp"

#include <windows.h>
#include <string>
#include <filesystem>

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
		const char* font_path = "c:\\msyh.ttc";
		if (std::filesystem::exists(font_path))
			io.Fonts->AddFontFromFileTTF(font_path, 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
		else 
		{
			char buffer[2000];
			sprintf(buffer, "û�з��������ļ�,�����޷�������ʾ����\t  �ļ�:%s ����:%s Դ������:%d\n", __FILE__, __FUNCTION__, __LINE__);
			MessageBoxA(NULL, buffer, "������Ϣ", NULL);
		}
	}

	//��Ⱦ�������
	void render_manager() noexcept 
	{
		ImGui::Begin(u8"CSGO��Ϸ����", &g_config.control.show_imgui);
		ImGui::Text(u8"Ins  ��ʾ/���ز˵�");

		ImGui::Checkbox(u8"����Թ⹦��", &g_config.control.glow);
		ImGui::Checkbox(u8"�л�Ƥ������", &g_config.control.skin);
		ImGui::Checkbox(u8"��Ҿٱ�����", &g_config.control.report);
		ImGui::Checkbox(u8"�������鹦��", &g_config.control.aim);
		ImGui::Checkbox(u8"�������ù���", &g_config.control.other);

		ImGui::End();
	}

	//��Ⱦ���溯��
	void render() noexcept 
	{
		if (!g_config.control.show_imgui) return;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		render_manager();
		render_skin();
		render_report();
		render_aim();
		render_other();


		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	//�л�Ƥ���˵�
	void render_skin() noexcept
	{
		if (!g_config.control.skin) return;
		ImGui::Begin(u8"Ƥ��");

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
	}

	//�ٱ���Ҳ˵�
	void render_report() noexcept
	{
		if (!g_config.control.report) return;
		ImGui::Begin(u8"�ٱ�");

		ImGui::RadioButton(u8"ȫ���ٱ�", &g_config.control.report_mode, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"�����ٱ�", &g_config.control.report_mode, 2);
		ImGui::Separator();

		ImGui::Checkbox(u8"�ٱ�����", &g_config.control.report_text_abuse);
		ImGui::Checkbox(u8"�ٱ�ɧ��", &g_config.control.report_grief);
		ImGui::Checkbox(u8"�ٱ�͸��", &g_config.control.report_wall_hack);
		ImGui::Checkbox(u8"�ٱ�����", &g_config.control.report_aim_bot);
		ImGui::Checkbox(u8"�ٱ�����", &g_config.control.report_speed_hack);
		ImGui::Separator();

		ImGui::SliderInt(u8"�ٱ�ʱ����", &g_config.control.report_interval, 5, 50);
		ImGui::Separator();
		if (ImGui::Button(u8"���¾ٱ�")) report_space::clear_report_list();
		ImGui::Separator();

		if (g_config.control.report_mode == 2 && g_config.control.report_players.size())
		{
			static int report_player_index = 0;
			ImGui::Combo(u8"ѡ��ٱ����", &report_player_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = g_config.control.report_players[idx].name;
				return true;
			}, nullptr, g_config.control.report_players.size(), 10);

			//��ȡ��ҵ�XUID
			if ((int)g_config.control.report_players.size() > report_player_index)
				g_config.control.report_player_xuid = g_config.control.report_players[report_player_index].xuid;
		}
		ImGui::End();
	}

	//��������˵�
	void render_aim() noexcept
	{
		if (!g_config.control.aim) return;
		ImGui::Begin(u8"����");

		ImGui::Checkbox(u8"��������", &g_config.control.aim_scoped);
		ImGui::Checkbox(u8"��ǹ����", &g_config.control.aim_fire);
		ImGui::Checkbox(u8"��������", &g_config.control.aim_quiet_step);
		ImGui::Separator();

		ImGui::Checkbox(u8"��׼���˺��Զ�����", &g_config.control.aim_auto_scoped);
		ImGui::Checkbox(u8"��׼���˺��Զ���ǹ", &g_config.control.aim_auto_fire);
		ImGui::Checkbox(u8"�����رտ���״̬", &g_config.control.aim_close_scoped);
		ImGui::Separator();

		ImGui::SliderFloat(u8"����΢��", &g_config.control.aim_offset, 0.0f, 30.0f);
		ImGui::SliderFloat(u8"���鷶Χ", &g_config.control.aim_max_angle, 5, 89);

		ImGui::End();
	}

	//�������ܲ˵�
	void render_other() noexcept
	{
		if (!g_config.control.other) return;
		ImGui::Begin(u8"����");

		ImGui::Checkbox(u8"��������", &g_config.control.other_again_jump);

		ImGui::End();
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