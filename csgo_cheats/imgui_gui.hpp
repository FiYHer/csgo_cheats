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
		srand((unsigned int)time(0));

		//����imgui������
		ImGui::CreateContext();

		//����Ϸ����
		ImGui_ImplWin32_Init(FindWindowW(L"Valve001", NULL));

		//����imguiΪ��ɫ
		ImGui::StyleColorsLight();
		ImGui::GetStyle().ScrollbarSize = 9.0f;
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
			g_config.control.language_english = 1;
		}
	}

	//��Ⱦ�������
	void render_manager() noexcept 
	{
		ImGui::Begin(u8"csgo assistant", &g_config.control.show_imgui);

		ImGui::Text(u8"language");
		ImGui::RadioButton(u8"������ʾ", &g_config.control.language_english, 0);ImGui::SameLine();
		ImGui::RadioButton(u8"english show", &g_config.control.language_english, 1);

		std::string tip1 = string_to_utf8("Ins  ��ʾ / ���ز˵�");
		std::string tip2 = string_to_utf8("��ʾ:���ǽ����˸���밴Ins���ص�ǰ�˵����ɽ��!!!");
		std::string show_glow = string_to_utf8("����Թ⹦��");
		std::string show_skin = string_to_utf8("�л�Ƥ������");
		std::string show_report = string_to_utf8("��Ҿٱ�����");
		std::string show_aim = string_to_utf8("�������鹦��");
		std::string show_other = string_to_utf8("�������ù���");

		if (g_config.control.language_english)
		{
			tip1 = "Ins  show / hide menu";
			tip2 = "tip: if the wall is flickering, press Ins to hide the current menu.";
			show_glow = "players glow functions";
			show_skin = "change skin functions";
			show_report = "players report functions";
			show_aim = "players aimbot functions";
			show_other = "other useful functions";
		}

		ImGui::Text(tip1.c_str());
		ImGui::Text(tip2.c_str());
		ImGui::Checkbox(show_glow.c_str(), &g_config.control.show_glow);
		ImGui::Checkbox(show_skin.c_str(), &g_config.control.show_skin);
		ImGui::Checkbox(show_report.c_str(), &g_config.control.show_report);
		ImGui::Checkbox(show_aim.c_str(), &g_config.control.aim_show);
		ImGui::Checkbox(show_other.c_str(), &g_config.control.show_other);

		ImGui::End();
	}

	//��Ⱦ���溯��
	void render() noexcept 
	{
		if (g_config.control.show_imgui)
		{
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			render_manager();
			render_glow();
			render_skin();
			render_report();
			render_aim();
			render_other();

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}
	}

	//����Թ�˵�
	void render_glow() noexcept
	{
		if (!g_config.control.show_glow) return;

		ImGui::Begin(u8"glow");

		std::string glow_enable = string_to_utf8("�����Թ�");
		std::string glow_enemy = string_to_utf8("�Թ����");
		std::string glow_friend = string_to_utf8("�Թ����");

		if (g_config.control.language_english)
		{
			glow_enable = "enable glow";
			glow_enemy = "glow enemy";
			glow_friend = "glow friend";
		}

		ImGui::Checkbox(glow_enable.c_str(), &g_config.control.glow_enable);
		ImGui::Checkbox(glow_enemy.c_str(), &g_config.control.glow_enemy);
		ImGui::Checkbox(glow_friend.c_str(), &g_config.control.glow_friend);

		ImGui::End();
	}

	//�л�Ƥ���˵�
	void render_skin() noexcept
	{
		if (!g_config.control.show_skin) return;
		ImGui::Begin(u8"skin");

		std::string skin_enable = string_to_utf8("��������");
		std::string skin_ = string_to_utf8("����Ƥ��ѡ��");
		std::string knife_ = string_to_utf8("С��ģ��ѡ��");

		if (g_config.control.language_english)
		{
			skin_enable = "enable skin";
			skin_ = "weapon skin select";
			knife_ = "knife model select";
		}

		ImGui::Checkbox(skin_enable.c_str(), &g_config.control.skin_enable);

		static int weapon_kit_index_last = 0;//��һ�ε�����Ƥ��
		static int weapon_kit_index = rand() % g_config.control.skin_vector.size();//��һ�ε�����Ƥ��

		ImGui::Combo(skin_.c_str(), &weapon_kit_index, [](void* data, int idx, const char** out_text)
		{
			//����Ƥ���ַ���
			*out_text = g_config.control.skin_vector[idx].name.c_str();

			//��������Ƥ��ID
			g_config.control.weapon_skin_id = g_config.control.skin_vector[idx].id;
			return true;
		}, nullptr, g_config.control.skin_vector.size(), 10);

		if (weapon_kit_index_last != weapon_kit_index)
		{
			skin_space::schedule_hud_update();
			weapon_kit_index_last = weapon_kit_index;
		}

		//С��ѡ��
		static int knife_select_last = 0;
		static int knife_select = rand() % g_config.control.knife_vector.size();

		ImGui::Combo(knife_.c_str(), &knife_select, [](void* data, int idx, const char** out_text)
		{
			//С��ģ���ַ���
			*out_text = g_config.control.knife_vector[idx].name.c_str();

			//
			g_config.control.knife_index = g_config.control.knife_vector[idx].id;
			return true;
		}, nullptr, g_config.control.knife_vector.size(), 10);

		if (knife_select != knife_select_last)
		{
			skin_space::schedule_hud_update();
			knife_select_last = knife_select;
		}

		ImGui::End();
	}

	//�ٱ���Ҳ˵�
	void render_report() noexcept
	{
		if (!g_config.control.show_report) return;
		ImGui::Begin(u8"report");

		std::string report_enable = string_to_utf8("�����ٱ�");
		std::string report_all = string_to_utf8("�ٱ�ȫ��");
		std::string report_per = string_to_utf8("�ٱ�һ��");
		std::string report_abuse = string_to_utf8("�ٱ�����");
		std::string report_grief = string_to_utf8("�ٱ�ɧ��");
		std::string report_wallhack = string_to_utf8("�ٱ�͸��");
		std::string report_aimhack = string_to_utf8("�ٱ�����");
		std::string report_speedhack = string_to_utf8("�ٱ�����");
		std::string report_player_name = string_to_utf8("ѡ����Ҫ�ٱ����������");
		std::string report_time_interval = string_to_utf8("�ٱ�ʱ����");
		std::string report_clear = string_to_utf8("��վٱ��б�");

		if (g_config.control.language_english)
		{
			report_enable = "enable report";
			report_all = "report all";
			report_per = "report per";
			report_abuse = "report abuse";
			report_grief = "report grief";
			report_wallhack = "report wallhack";
			report_aimhack = "report aimhack";
			report_speedhack = "report speedhack";
			report_player_name = "select report player";
			report_time_interval = "report interval";
			report_clear = "clear report list";
		}

		ImGui::Checkbox(report_enable.c_str(), &g_config.control.report_enable);
		ImGui::RadioButton(report_all.c_str(), &g_config.control.report_mode, 1); ImGui::SameLine();
		ImGui::RadioButton(report_per.c_str(), &g_config.control.report_mode, 2); ImGui::Separator();
		ImGui::Checkbox(report_abuse.c_str(), &g_config.control.report_text_abuse);
		ImGui::Checkbox(report_grief.c_str(), &g_config.control.report_grief);
		ImGui::Checkbox(report_wallhack.c_str(), &g_config.control.report_wall_hack);
		ImGui::Checkbox(report_aimhack.c_str(), &g_config.control.report_aim_bot);
		ImGui::Checkbox(report_speedhack.c_str(), &g_config.control.report_speed_hack);
		ImGui::Separator();

		if (g_config.control.report_mode == 2 && g_config.control.report_players.size())
		{
			static int report_player_index = 0;
			ImGui::Combo(report_player_name.c_str(), &report_player_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = g_config.control.report_players[idx].name;
				return true;
			}, nullptr, g_config.control.report_players.size(), 10);

			//��ȡ��ҵ�XUID
			if ((int)g_config.control.report_players.size() > report_player_index)
				g_config.control.report_player_xuid = g_config.control.report_players[report_player_index].xuid;
		}

		ImGui::SliderInt(report_time_interval.c_str(), &g_config.control.report_interval, 5, 50); ImGui::Separator();
		if (ImGui::Button(report_clear.c_str())) report_space::clear_report_list();

		ImGui::End();
	}

	//��������˵�
	void render_aim() noexcept
	{
		if (!g_config.control.aim_show) return;
		ImGui::Begin(u8"aimbot");

		std::string enable_aim = string_to_utf8("��������");
		std::string aim_head = string_to_utf8("����ͷ��");
		std::string aim_sternum = string_to_utf8("�����ز�");
		std::string aim_recent = string_to_utf8("�������");
		std::string aim_auto_shot = string_to_utf8("�Զ���ǹ");
		std::string aim_auto_scope = string_to_utf8("�Զ�����");
		std::string aim_aimlock = string_to_utf8("��������");
		std::string aim_fov = string_to_utf8("���鷶Χ");
		std::string aim_aim_inaccuracy = string_to_utf8("��������������ȷ��");
		std::string aim_shot_inaccurac = string_to_utf8("��������������ȷ��");
		std::string aim_offset = string_to_utf8("����΢��");

		if (g_config.control.language_english)
		{
			enable_aim = "enable aimbot";
			aim_head = "aimbot head";
			aim_sternum = "aimbot sternum";
			aim_recent = "aimbot recent";
			aim_auto_shot = "auto shot";
			aim_auto_scope = "auto scope";
			aim_aimlock = "aimbot lock";
			aim_fov = "aimbot fov";
			aim_aim_inaccuracy = "aimbot inaccuracy";
			aim_shot_inaccurac = "shot inaccurac";
			aim_offset = "aimbot offset";
		}

		ImGui::Checkbox(enable_aim.c_str(), &g_config.control.aim_enable);
		ImGui::RadioButton(aim_head.c_str(), &g_config.control.aim_bone, 8); ImGui::SameLine();
		ImGui::RadioButton(aim_sternum.c_str(), &g_config.control.aim_bone, 6); ImGui::SameLine();
		ImGui::RadioButton(aim_recent.c_str(), &g_config.control.aim_bone, -1);
		ImGui::Checkbox(aim_auto_shot.c_str(), &g_config.control.aim_auto_shot);
		ImGui::Checkbox(aim_auto_scope.c_str(), &g_config.control.aim_auto_scope);
		ImGui::Checkbox(aim_aimlock.c_str(), &g_config.control.aim_aimlock);
		ImGui::SliderFloat(aim_fov.c_str(), &g_config.control.aim_fov, 0.0f, 255.0f);
		ImGui::SliderFloat(aim_aim_inaccuracy.c_str(), &g_config.control.aim_max_aim_inaccuracy, 0.0f, 1.0f);
		ImGui::SliderFloat(aim_shot_inaccurac.c_str(), &g_config.control.aim_max_shot_inaccuracy, 0.0f, 1.0f);
		ImGui::SliderFloat(aim_offset.c_str(), &g_config.control.aim_offset, -20.0f, 20.0f);

		ImGui::End();
	}

	//�������ܲ˵�
	void render_other() noexcept
	{
		if (!g_config.control.show_other) return;
		ImGui::Begin(u8"other");

		if(!g_config.control.language_english) ImGui::Checkbox(u8"��������", &g_config.control.other_again_jump);
		else ImGui::Checkbox(u8"rabbit jump", &g_config.control.other_again_jump);

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