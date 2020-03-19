#pragma once
#include "config.hpp"
#include "skin.hpp"
#include "aim.hpp"
#include "report.hpp"
#include "imgui_gui.hpp"
#include "glow.hpp"
#include "other.hpp"

#include <assert.h>
#include <Psapi.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <d3d9.h>
#pragma comment(lib,"d3d9")

//前向声明
static LRESULT __stdcall my_window_proc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
static HRESULT __stdcall my_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept;
static HRESULT __stdcall my_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
static int __stdcall do_post_screen_effects(int param) noexcept;
static void __stdcall frame_stage_notify(frame_stage_enum stage) noexcept;
static bool __stdcall create_move(float input_sample_time, user_cmd_struct* cmd) noexcept;


//钩子类
class hooks_class
{
public:
	//虚拟内存钩子类
	class vmt_class
	{
	private:
		void* m_base;//内存基址
		uintptr_t* m_old_vmt;//原始的地址
		uintptr_t* m_new_vmt;//新的地址
		size_t m_length;//地址页面长度

		//计算有效页面数量
		size_t get_use_memory_length(uintptr_t* vmt) noexcept
		{
			size_t length = 0;

			//获取该地址的页面信息，该页面能读取能执行
			MEMORY_BASIC_INFORMATION memoryInfo;
			while (VirtualQuery(LPCVOID(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ) length++;
			
			return length;
		}

		//查找可用的空闲内存地址页面
		uintptr_t* get_free_memory_page(void* const base, int length) noexcept
		{
			//获取内存相关信息
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(base, &mbi, sizeof(mbi));

			//获取模块信息
			MODULEINFO moduleInfo;
			GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(mbi.AllocationBase), &moduleInfo, sizeof(moduleInfo));

			//取得模块结束地址
			auto moduleEnd{ reinterpret_cast<uintptr_t*>(static_cast<std::byte*>(moduleInfo.lpBaseOfDll) + moduleInfo.SizeOfImage) };

			for (auto currentAddress = moduleEnd - length; currentAddress > moduleInfo.lpBaseOfDll; currentAddress -= *currentAddress ? length : 1)
				if (!*currentAddress)//地址有效
					if (VirtualQuery(currentAddress, &mbi, sizeof(mbi)) && mbi.State == MEM_COMMIT//内存地址预定了
						&& mbi.Protect == PAGE_READWRITE && mbi.RegionSize >= length * sizeof(uintptr_t)//能读能写
						&& std::all_of(currentAddress, currentAddress + length, [](uintptr_t a) { return !a; }))
						return currentAddress;

			return nullptr;
		}
	public:
		vmt_class(void* const base) noexcept
		{
			//保证传进来的内存基址不为空
			assert(base);

			//保存内存基址
			m_base = base;

			//获取原始页面地址
			m_old_vmt = *reinterpret_cast<uintptr_t**>(base);

			//获取可用页面数量
			m_length = get_use_memory_length(m_old_vmt) + 1;

			//获取新的可用页面地址
			m_new_vmt = get_free_memory_page(base, m_length);
			assert(m_new_vmt);

			//将原始内存页面的数据全部复制到新的内存页面
			std::copy(m_old_vmt - 1, m_old_vmt - 1 + m_length, m_new_vmt);

			//用新内存页面覆盖原始内存页面
			*reinterpret_cast<uintptr_t**>(base) = m_new_vmt + 1;
		}

		//恢复原始地址
		void restore() noexcept
		{
			//还原地址
			if (m_base && m_old_vmt)
				*reinterpret_cast<uintptr_t**>(m_base) = m_old_vmt;
			//清空相关内存
			if (m_new_vmt)
				ZeroMemory(m_new_vmt, m_length * sizeof(uintptr_t));
		}

		//用指定函数地址覆盖指定内存页面的地址
		template<typename T>
		void hook_at(size_t index, T fun) const noexcept
		{
			if (index <= m_length)
				m_new_vmt[index + 1] = reinterpret_cast<uintptr_t>(fun);
		}

		//调用原始页面地址
		template<typename T, std::size_t Idx, typename ...Args>
		constexpr auto call_original(Args... args) const noexcept
		{
			return reinterpret_cast<T(__thiscall*)(void*, Args...)>(m_old_vmt[Idx])(m_base, args...);
		}

		//获取原始页面地址
		template<typename T, typename ...Args>
		constexpr auto get_original(size_t index, Args... args) const noexcept
		{
			return reinterpret_cast<T(__thiscall*)(void*, Args...)>(m_old_vmt[index]);
		}
	};

public:
	hooks_class() noexcept
	{
		//武器皮肤初始化
		skin_space::initialize_skin();
		skin_space::initialize_weapon();
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
		_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

		//替换游戏窗口过程
		original_window_proc = WNDPROC(SetWindowLongPtrA(FindWindowW(L"Valve001", nullptr), GWLP_WNDPROC, LONG_PTR(my_window_proc)));
	
		//替换present和reset函数
		original_present = **reinterpret_cast<decltype(original_present)**>(g_config.memory.present);
		**reinterpret_cast<decltype(my_present)***>(g_config.memory.present) = my_present;
		original_reset = **reinterpret_cast<decltype(original_reset)**>(g_config.memory.reset);
		**reinterpret_cast<decltype(my_reset)***>(g_config.memory.reset) = my_reset;

		client.hook_at(37, frame_stage_notify);
		clientMode.hook_at(24, create_move);
		clientMode.hook_at(44, do_post_screen_effects);

		g_config.gmae_ui->message_box("good message", "csgo helper inject finish");
	}

	//恢复游戏原数据
	void restore() noexcept
	{
		clientMode.restore();
		client.restore();

		glow_space::clear_custom_objects();

		SetWindowLongPtrA(FindWindowW(L"Valve001", nullptr), GWLP_WNDPROC, LONG_PTR(original_window_proc));
		**reinterpret_cast<void***>(g_config.memory.present) = original_present;
		**reinterpret_cast<void***>(g_config.memory.reset) = original_reset;
	}

	vmt_class clientMode{ g_config.client_mode };//客户模式hook
	vmt_class client{ g_config.client };//客户端hook

	//原始游戏窗口过程
	WNDPROC original_window_proc;

	//Present
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> original_present;
	
	//Reset
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> original_reset;
};

extern hooks_class g_hooks;

//我们准备的窗口过程
static LRESULT __stdcall my_window_proc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//声明imgui里面的窗口过程
	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//进入imgui里面进行消息处理
	ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);

	//控制imgui的显示和隐藏
	if (msg == WM_KEYDOWN && wParam == VK_INSERT) g_config.control.show_imgui = !g_config.control.show_imgui;

	//回到原始的游戏窗口地址
	return CallWindowProc(g_hooks.original_window_proc, window, msg, wParam, lParam);
}

//我们自己的present函数
static HRESULT __stdcall my_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
{
	//初始化imgui的DX9设备
	static bool imguiInit{ ImGui_ImplDX9_Init(device) };

	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	IDirect3DVertexDeclaration9* vertexDeclaration;
	device->GetVertexDeclaration(&vertexDeclaration);

	g_imgui.render();

	device->SetVertexDeclaration(vertexDeclaration);
	vertexDeclaration->Release();

	return g_hooks.original_present(device, src, dest, windowOverride, dirtyRegion);
}

//我们自己的reset函数
static HRESULT __stdcall my_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = g_hooks.original_reset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

static int __stdcall do_post_screen_effects(int param) noexcept
{
	if (g_config.engine->is_in_game())
	{
		glow_space::render();
	}
	return g_hooks.clientMode.call_original<int, 44>(param);
}

static void __stdcall frame_stage_notify(frame_stage_enum stage) noexcept
{
	if (g_config.engine->is_in_game())
	{
		skin_space::run(stage);
	}
	g_hooks.client.call_original<void, 37>(stage);
}

static bool __stdcall create_move(float input_sample_time, user_cmd_struct* cmd) noexcept
{
	bool state = g_hooks.clientMode.call_original<bool, 24>(input_sample_time, cmd);

	//没有命令
	if (!cmd->command_number) return state;

	if (g_config.engine->is_in_game())
	{
		aim_space::run(cmd);
		report_space::run();
		other_space::again_jump(cmd);
	}

	return state;
}



