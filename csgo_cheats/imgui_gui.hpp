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
			g_config.control.language_english = 1;
		}
	}

	//渲染界面管理
	void render_manager() noexcept 
	{
		ImGui::Begin(u8"csgo assistant", &g_config.control.show_imgui);

		ImGui::Text(u8"language");
		ImGui::RadioButton(u8"中文显示", &g_config.control.language_english, 0);ImGui::SameLine();
		ImGui::RadioButton(u8"english show", &g_config.control.language_english, 1);

		std::string tip1 = string_to_utf8("Ins  显示 / 隐藏菜单");
		std::string tip2 = string_to_utf8("提示:如果墙体闪烁，请按Ins隐藏当前菜单即可解决!!!");
		std::string show_glow = string_to_utf8("人物辉光功能");
		std::string show_skin = string_to_utf8("切换皮肤功能");
		std::string show_report = string_to_utf8("玩家举报功能");
		std::string show_aim = string_to_utf8("人物自瞄功能");
		std::string show_other = string_to_utf8("其它有用功能");

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
		if (!g_config.control.show_glow) return;

		ImGui::Begin(u8"glow");

		std::string glow_enable = string_to_utf8("开启辉光");
		std::string glow_enemy = string_to_utf8("辉光敌人");
		std::string glow_friend = string_to_utf8("辉光队友");

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

	//切换皮肤菜单
	void render_skin() noexcept
	{
		if (!g_config.control.show_skin) return;
		ImGui::Begin(u8"skin");

		std::string skin_enable = string_to_utf8("开启换肤");
		std::string skin_ = string_to_utf8("武器皮肤选择");
		std::string knife_ = string_to_utf8("小刀模型选择");

		if (g_config.control.language_english)
		{
			skin_enable = "enable skin";
			skin_ = "weapon skin select";
			knife_ = "knife model select";
		}

		ImGui::Checkbox(skin_enable.c_str(), &g_config.control.skin_enable);

		static int weapon_kit_index_last = 0;//上一次的武器皮肤
		static int weapon_kit_index = rand() % g_config.control.skin_vector.size();//这一次的武器皮肤

		ImGui::Combo(skin_.c_str(), &weapon_kit_index, [](void* data, int idx, const char** out_text)
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

		//小刀选择
		static int knife_select_last = 0;
		static int knife_select = rand() % g_config.control.knife_vector.size();

		ImGui::Combo(knife_.c_str(), &knife_select, [](void* data, int idx, const char** out_text)
		{
			//小刀模型字符串
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

	//举报玩家菜单
	void render_report() noexcept
	{
		if (!g_config.control.show_report) return;
		ImGui::Begin(u8"report");

		std::string report_enable = string_to_utf8("开启举报");
		std::string report_all = string_to_utf8("举报全部");
		std::string report_per = string_to_utf8("举报一个");
		std::string report_abuse = string_to_utf8("举报骂人");
		std::string report_grief = string_to_utf8("举报骚扰");
		std::string report_wallhack = string_to_utf8("举报透视");
		std::string report_aimhack = string_to_utf8("举报自瞄");
		std::string report_speedhack = string_to_utf8("举报加速");
		std::string report_player_name = string_to_utf8("选择需要举报的玩家名字");
		std::string report_time_interval = string_to_utf8("举报时间间隔");
		std::string report_clear = string_to_utf8("清空举报列表");

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

			//获取玩家的XUID
			if ((int)g_config.control.report_players.size() > report_player_index)
				g_config.control.report_player_xuid = g_config.control.report_players[report_player_index].xuid;
		}

		ImGui::SliderInt(report_time_interval.c_str(), &g_config.control.report_interval, 5, 50); ImGui::Separator();
		if (ImGui::Button(report_clear.c_str())) report_space::clear_report_list();

		ImGui::End();
	}

	//自瞄人物菜单
	void render_aim() noexcept
	{
		if (!g_config.control.aim_show) return;
		ImGui::Begin(u8"aimbot");

		std::string enable_aim = string_to_utf8("开启自瞄");
		std::string aim_head = string_to_utf8("自瞄头部");
		std::string aim_sternum = string_to_utf8("自瞄胸部");
		std::string aim_recent = string_to_utf8("自瞄最近");
		std::string aim_auto_shot = string_to_utf8("自动开枪");
		std::string aim_auto_scope = string_to_utf8("自动开镜");
		std::string aim_aimlock = string_to_utf8("锁定敌人");
		std::string aim_fov = string_to_utf8("自瞄范围");
		std::string aim_aim_inaccuracy = string_to_utf8("允许自瞄的最大不正确率");
		std::string aim_shot_inaccurac = string_to_utf8("允许射击的最大不正确率");
		std::string aim_offset = string_to_utf8("自瞄微调");

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

	//其它功能菜单
	void render_other() noexcept
	{
		if (!g_config.control.show_other) return;
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