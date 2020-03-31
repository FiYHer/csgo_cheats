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
		ImGui::GetStyle().ScrollbarSize = 9.0f;
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

	//渲染界面管理
	void render_manager() noexcept 
	{
		ImGui::Begin(u8"csgo assistant", &g_config.control.show_imgui);

		ImGui::Text(u8"Language");
		ImGui::SameLine();
		ImGui::RadioButton(u8"中文显示", &g_config.control.language_english, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"English show", &g_config.control.language_english, 1);

		if (!g_config.control.language_english)
		{
			ImGui::Text(u8"Ins  显示 / 隐藏菜单");
			ImGui::Text(u8"提示:如果墙体闪烁，请按Ins隐藏当前菜单即可解决!!!");
			ImGui::Checkbox(u8"人物辉光功能", &g_config.control.glow);
			ImGui::Checkbox(u8"切换皮肤功能", &g_config.control.skin);
			ImGui::Checkbox(u8"玩家举报功能", &g_config.control.report);
			ImGui::Checkbox(u8"人物自瞄功能", &g_config.control.aim);
			ImGui::Checkbox(u8"其它有用功能", &g_config.control.other);
		}
		else
		{
			ImGui::Text(u8"Ins  show / hide menu");
			ImGui::Text(u8"tip: if the wall is flickering, press Ins to hide the current menu.");
			ImGui::Checkbox(u8"players glow functions", &g_config.control.glow);
			ImGui::Checkbox(u8"change skin functions", &g_config.control.skin);
			ImGui::Checkbox(u8"players report functions", &g_config.control.report);
			ImGui::Checkbox(u8"players aimbot functions", &g_config.control.aim);
			ImGui::Checkbox(u8"other useful functions", &g_config.control.other);
		}

		ImGui::End();
	}

	//渲染界面函数
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

	//人物辉光菜单
	void render_glow() noexcept
	{
		if (!g_config.control.glow) return;

		ImGui::Begin(u8"glow");

		if (!g_config.control.language_english)
		{
			ImGui::Checkbox(u8"辉光敌人", &g_config.control.glow_enemy);
			ImGui::Checkbox(u8"辉光队友", &g_config.control.glow_friend);
		}
		else
		{
			ImGui::Checkbox(u8"enemy glow", &g_config.control.glow_enemy);
			ImGui::Checkbox(u8"teammate glow", &g_config.control.glow_friend);
		}

		ImGui::End();
	}

	//切换皮肤菜单
	void render_skin() noexcept
	{
		if (!g_config.control.skin) return;
		ImGui::Begin(u8"skin");

		static int weapon_kit_index_last = 0;//上一次的武器皮肤
		static int weapon_kit_index = 0;//这一次的武器皮肤

		if (!g_config.control.language_english)
		{
			ImGui::Combo(u8"武器皮肤选择", &weapon_kit_index, [](void* data, int idx, const char** out_text)
			{
				//武器皮肤字符串
				*out_text = g_config.control.skin_vector[idx].name.c_str();

				//更新武器皮肤ID
				g_config.control.weapon_skin_id = g_config.control.skin_vector[idx].id;
				return true;
			}, nullptr, g_config.control.skin_vector.size(), 10);
			if (weapon_kit_index_last != weapon_kit_index)
			{
				skin_space::schedule_hud_update();
				weapon_kit_index_last = weapon_kit_index;
			}
		}
		else
		{
			ImGui::Combo(u8"weapon skin select", &weapon_kit_index, [](void* data, int idx, const char** out_text)
			{
				//武器皮肤字符串
				*out_text = g_config.control.skin_vector[idx].name.c_str();

				//更新武器皮肤ID
				g_config.control.weapon_skin_id = g_config.control.skin_vector[idx].id;
				return true;
			}, nullptr, g_config.control.skin_vector.size(), 10);
			if (weapon_kit_index_last != weapon_kit_index)
			{
				skin_space::schedule_hud_update();
				weapon_kit_index_last = weapon_kit_index;
			}
		}

		//小刀选择
		static int knife_select_last = 0;
		static int knife_select = 0;

		if (!g_config.control.language_english)
		{
			ImGui::Combo(u8"小刀模型选择", &knife_select, [](void* data, int idx, const char** out_text)
			{
				//武器皮肤字符串
				*out_text = g_config.control.knife_vector[idx].name.c_str();

				//更新武器皮肤ID
				g_config.control.knife_index = g_config.control.knife_vector[idx].id;
				return true;
			}, nullptr, g_config.control.knife_vector.size(), 10);
			if (knife_select != knife_select_last)
			{
				skin_space::schedule_hud_update();
				knife_select_last = knife_select;
			}
		}
		else
		{
			ImGui::Combo(u8"knife model select", &knife_select, [](void* data, int idx, const char** out_text)
			{
				//武器皮肤字符串
				*out_text = g_config.control.knife_vector[idx].name.c_str();

				//更新武器皮肤ID
				g_config.control.knife_index = g_config.control.knife_vector[idx].id;
				return true;
			}, nullptr, g_config.control.knife_vector.size(), 10);
			if (knife_select != knife_select_last)
			{
				skin_space::schedule_hud_update();
				knife_select_last = knife_select;
			}
		}

		ImGui::End();
	}

	//举报玩家菜单
	void render_report() noexcept
	{
		if (!g_config.control.report) return;
		ImGui::Begin(u8"report");

		if (!g_config.control.language_english)
		{
			ImGui::RadioButton(u8"全部举报", &g_config.control.report_mode, 1); ImGui::SameLine();
			ImGui::RadioButton(u8"单个举报", &g_config.control.report_mode, 2); ImGui::Separator();
			ImGui::Checkbox(u8"举报骂人", &g_config.control.report_text_abuse);
			ImGui::Checkbox(u8"举报骚扰", &g_config.control.report_grief);
			ImGui::Checkbox(u8"举报透视", &g_config.control.report_wall_hack);
			ImGui::Checkbox(u8"举报自瞄", &g_config.control.report_aim_bot);
			ImGui::Checkbox(u8"举报加速", &g_config.control.report_speed_hack);
			ImGui::Separator();
		}
		else
		{
			ImGui::RadioButton(u8"report everyone", &g_config.control.report_mode, 1); ImGui::SameLine();
			ImGui::RadioButton(u8"report one", &g_config.control.report_mode, 2); ImGui::Separator();
			ImGui::Checkbox(u8"report text abuse", &g_config.control.report_text_abuse);
			ImGui::Checkbox(u8"report grief", &g_config.control.report_grief);
			ImGui::Checkbox(u8"report wall hack", &g_config.control.report_wall_hack);
			ImGui::Checkbox(u8"report aimbot hack", &g_config.control.report_aim_bot);
			ImGui::Checkbox(u8"report speed hack", &g_config.control.report_speed_hack);
			ImGui::Separator();
		}

		if (!g_config.control.language_english)
		{
			if (g_config.control.report_mode == 2 && g_config.control.report_players.size())
			{
				static int report_player_index = 0;
				ImGui::Combo(u8"选择举报玩家", &report_player_index, [](void* data, int idx, const char** out_text)
				{
					*out_text = g_config.control.report_players[idx].name;
					return true;
				}, nullptr, g_config.control.report_players.size(), 10);

				//获取玩家的XUID
				if ((int)g_config.control.report_players.size() > report_player_index)
					g_config.control.report_player_xuid = g_config.control.report_players[report_player_index].xuid;
			}
		}
		else
		{
			if (g_config.control.report_mode == 2 && g_config.control.report_players.size())
			{
				static int report_player_index = 0;
				ImGui::Combo(u8"select report player name", &report_player_index, [](void* data, int idx, const char** out_text)
				{
					*out_text = g_config.control.report_players[idx].name;
					return true;
				}, nullptr, g_config.control.report_players.size(), 10);

				//获取玩家的XUID
				if ((int)g_config.control.report_players.size() > report_player_index)
					g_config.control.report_player_xuid = g_config.control.report_players[report_player_index].xuid;
			}
		}

		if (!g_config.control.language_english)
		{
			ImGui::SliderInt(u8"举报时间间隔", &g_config.control.report_interval, 5, 50);ImGui::Separator();
			if (ImGui::Button(u8"清空举报列表")) report_space::clear_report_list();
		}
		else
		{
			ImGui::SliderInt(u8"report time interval", &g_config.control.report_interval, 5, 50); ImGui::Separator();
			if (ImGui::Button(u8"clear report list")) report_space::clear_report_list();
		}

		ImGui::End();
	}

	//自瞄人物菜单
	void render_aim() noexcept
	{
		if (!g_config.control.aim) return;
		ImGui::Begin(u8"aimbot");

		if (!g_config.control.language_english)
		{
			ImGui::Checkbox(u8"开镜自瞄", &g_config.control.aim_scoped);
			ImGui::Checkbox(u8"开枪自瞄", &g_config.control.aim_fire);
			ImGui::Checkbox(u8"静步自瞄", &g_config.control.aim_quiet_step); ImGui::Separator();
			ImGui::Checkbox(u8"瞄准敌人后自动开镜", &g_config.control.aim_auto_scoped);
			ImGui::Checkbox(u8"瞄准敌人后自动开枪", &g_config.control.aim_auto_fire);
			ImGui::Checkbox(u8"自瞄后关闭开镜状态", &g_config.control.aim_close_scoped); ImGui::Separator();
			ImGui::SliderFloat(u8"自瞄微调", &g_config.control.aim_offset, 0.0f, 30.0f);
			ImGui::SliderFloat(u8"自瞄范围", &g_config.control.aim_max_angle, 5, 89);
		}
		else
		{
			ImGui::Checkbox(u8"aimbot when opening the mirror", &g_config.control.aim_scoped);
			ImGui::Checkbox(u8"aimbot when shooting", &g_config.control.aim_fire);
			ImGui::Checkbox(u8"aimbot when the button shift is pressed", &g_config.control.aim_quiet_step); ImGui::Separator();
			ImGui::Checkbox(u8"automatically turn on the mirror after aiming at the enemy", &g_config.control.aim_auto_scoped);
			ImGui::Checkbox(u8"automatically fire after aiming at the enemy", &g_config.control.aim_auto_fire);
			ImGui::Checkbox(u8"turn off the mirror state after aimbot", &g_config.control.aim_close_scoped); ImGui::Separator();
			ImGui::SliderFloat(u8"aimbot pos fine tuning", &g_config.control.aim_offset, 0.0f, 30.0f);
			ImGui::SliderFloat(u8"aimbot range fine tuning", &g_config.control.aim_max_angle, 5, 89);
		}

		ImGui::End();
	}

	//其它功能菜单
	void render_other() noexcept
	{
		if (!g_config.control.other) return;
		ImGui::Begin(u8"other");

		if(!g_config.control.language_english) ImGui::Checkbox(u8"连跳功能", &g_config.control.other_again_jump);
		else ImGui::Checkbox(u8"rabbit jump", &g_config.control.other_again_jump);

		ImGui::End();
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