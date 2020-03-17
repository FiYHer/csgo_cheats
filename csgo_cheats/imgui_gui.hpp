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
		//创建imgui上下文
		ImGui::CreateContext();

		//绑定游戏窗口
		ImGui_ImplWin32_Init(FindWindowW(L"Valve001", NULL));

		//设置imgui为白色
		ImGui::StyleColorsLight();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;//不需要保存imgui配置文件
		io.LogFilename = nullptr;//不需要保存imgui信息
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;//不需要改变鼠标样式

		//设置字体文件
		const char* font_path = "c:\\msyh.ttc";
		if (std::filesystem::exists(font_path))
			io.Fonts->AddFontFromFileTTF(font_path, 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
		else 
		{
			char buffer[2000];
			sprintf(buffer, "没有发现字体文件,可能无法正常显示字体\t  文件:%s 函数:%s 源代码行:%d\n", __FILE__, __FUNCTION__, __LINE__);
			MessageBoxA(NULL, buffer, "警告信息", NULL);
		}
	}

	//渲染界面函数
	void render() noexcept 
	{
		if (!g_config.control.show_imgui) return;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(u8"CSGO游戏辅助", &g_config.control.show_imgui);

		ImGui::Checkbox(u8"辉光人物", &g_config.control.glow);
		ImGui::Separator();

		static int weapon_kit_index_last = 0;//上一次的武器皮肤
		static int weapon_kit_index = 0;//这一次的武器皮肤
		ImGui::Checkbox(u8"武器换皮肤", &g_config.control.skin);
		ImGui::SameLine();
		ImGui::Combo(u8"武器皮肤选择", &weapon_kit_index, [](void* data, int idx, const char** out_text)
		{
			//武器皮肤字符串
			*out_text = g_config.control.skin_vector[idx].name.c_str();

			//更新武器皮肤ID
			g_config.control.weapon_skin_id = g_config.control.skin_vector[idx].id;
			return true;
		}, nullptr, g_config.control.skin_vector.size(), 10);

		//这一次于上一次不一样，所以是新选择了武器皮肤
		if (weapon_kit_index_last != weapon_kit_index)
		{
			skin_space::schedule_hud_update();
			weapon_kit_index_last = weapon_kit_index;
		}
		ImGui::Separator();

		ImGui::RadioButton(u8"关闭举报", &g_config.control.report_mode, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"全部举报", &g_config.control.report_mode, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"单个举报", &g_config.control.report_mode, 2);
		ImGui::Separator();

		if (g_config.control.report_mode)
		{
			ImGui::Checkbox(u8"举报骂人", &g_config.control.report_text_abuse);
			ImGui::SameLine();
			ImGui::Checkbox(u8"举报骚扰", &g_config.control.report_grief);
			ImGui::SameLine();
			ImGui::Checkbox(u8"举报透视", &g_config.control.report_wall_hack);
			ImGui::SameLine();
			ImGui::Checkbox(u8"举报自瞄", &g_config.control.report_aim_bot);
			ImGui::SameLine();
			ImGui::Checkbox(u8"举报加速", &g_config.control.report_speed_hack);
			ImGui::Separator();

			ImGui::SliderInt(u8"举报时间间隔", &g_config.control.report_interval, 5, 50);
			ImGui::SameLine();
			if (ImGui::Button(u8"从新举报")) report_space::clear_report_list();
			ImGui::Separator();

			if (g_config.control.report_mode == 2 && g_config.control.report_players.size())
			{
				static int report_player_index = 0;
				ImGui::Combo(u8"选择举报玩家", &report_player_index, [](void* data, int idx, const char** out_text)
				{
					//武器皮肤字符串
					*out_text = g_config.control.report_players[idx].name;

					//更新武器皮肤ID
					g_config.control.report_player_xuid = g_config.control.report_players[idx].xuid;
					return true;
				}, nullptr, g_config.control.report_players.size(), 10);
			}
		}



		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	//将文本转化为utf8使得imgui能正常显示中文
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

	//将utf8数据转化为字符串
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