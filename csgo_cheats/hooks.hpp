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

//ǰ������
static LRESULT __stdcall my_window_proc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
static HRESULT __stdcall my_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept;
static HRESULT __stdcall my_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
static int __stdcall do_post_screen_effects(int param) noexcept;
static void __stdcall frame_stage_notify(frame_stage_enum stage) noexcept;
static bool __stdcall create_move(float input_sample_time, user_cmd_struct* cmd) noexcept;


//������
class hooks_class
{
public:
	//�����ڴ湳����
	class vmt_class
	{
	private:
		void* m_base;//�ڴ��ַ
		uintptr_t* m_old_vmt;//ԭʼ�ĵ�ַ
		uintptr_t* m_new_vmt;//�µĵ�ַ
		size_t m_length;//��ַҳ�泤��

		//������Чҳ������
		size_t get_use_memory_length(uintptr_t* vmt) noexcept
		{
			size_t length = 0;

			//��ȡ�õ�ַ��ҳ����Ϣ����ҳ���ܶ�ȡ��ִ��
			MEMORY_BASIC_INFORMATION memoryInfo;
			while (VirtualQuery(LPCVOID(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ) length++;
			
			return length;
		}

		//���ҿ��õĿ����ڴ��ַҳ��
		uintptr_t* get_free_memory_page(void* const base, int length) noexcept
		{
			//��ȡ�ڴ������Ϣ
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(base, &mbi, sizeof(mbi));

			//��ȡģ����Ϣ
			MODULEINFO moduleInfo;
			GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(mbi.AllocationBase), &moduleInfo, sizeof(moduleInfo));

			//ȡ��ģ�������ַ
			auto moduleEnd{ reinterpret_cast<uintptr_t*>(static_cast<std::byte*>(moduleInfo.lpBaseOfDll) + moduleInfo.SizeOfImage) };

			for (auto currentAddress = moduleEnd - length; currentAddress > moduleInfo.lpBaseOfDll; currentAddress -= *currentAddress ? length : 1)
				if (!*currentAddress)//��ַ��Ч
					if (VirtualQuery(currentAddress, &mbi, sizeof(mbi)) && mbi.State == MEM_COMMIT//�ڴ��ַԤ����
						&& mbi.Protect == PAGE_READWRITE && mbi.RegionSize >= length * sizeof(uintptr_t)//�ܶ���д
						&& std::all_of(currentAddress, currentAddress + length, [](uintptr_t a) { return !a; }))
						return currentAddress;

			return nullptr;
		}
	public:
		vmt_class(void* const base) noexcept
		{
			//��֤���������ڴ��ַ��Ϊ��
			assert(base);

			//�����ڴ��ַ
			m_base = base;

			//��ȡԭʼҳ���ַ
			m_old_vmt = *reinterpret_cast<uintptr_t**>(base);

			//��ȡ����ҳ������
			m_length = get_use_memory_length(m_old_vmt) + 1;

			//��ȡ�µĿ���ҳ���ַ
			m_new_vmt = get_free_memory_page(base, m_length);
			assert(m_new_vmt);

			//��ԭʼ�ڴ�ҳ�������ȫ�����Ƶ��µ��ڴ�ҳ��
			std::copy(m_old_vmt - 1, m_old_vmt - 1 + m_length, m_new_vmt);

			//�����ڴ�ҳ�渲��ԭʼ�ڴ�ҳ��
			*reinterpret_cast<uintptr_t**>(base) = m_new_vmt + 1;
		}

		//�ָ�ԭʼ��ַ
		void restore() noexcept
		{
			//��ԭ��ַ
			if (m_base && m_old_vmt)
				*reinterpret_cast<uintptr_t**>(m_base) = m_old_vmt;
			//�������ڴ�
			if (m_new_vmt)
				ZeroMemory(m_new_vmt, m_length * sizeof(uintptr_t));
		}

		//��ָ��������ַ����ָ���ڴ�ҳ��ĵ�ַ
		template<typename T>
		void hook_at(size_t index, T fun) const noexcept
		{
			if (index <= m_length)
				m_new_vmt[index + 1] = reinterpret_cast<uintptr_t>(fun);
		}

		//����ԭʼҳ���ַ
		template<typename T, std::size_t Idx, typename ...Args>
		constexpr auto call_original(Args... args) const noexcept
		{
			return reinterpret_cast<T(__thiscall*)(void*, Args...)>(m_old_vmt[Idx])(m_base, args...);
		}

		//��ȡԭʼҳ���ַ
		template<typename T, typename ...Args>
		constexpr auto get_original(size_t index, Args... args) const noexcept
		{
			return reinterpret_cast<T(__thiscall*)(void*, Args...)>(m_old_vmt[index]);
		}
	};

public:
	hooks_class() noexcept
	{
		//����Ƥ����ʼ��
		skin_space::initialize_skin();
		skin_space::initialize_weapon();
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
		_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

		//�滻��Ϸ���ڹ���
		original_window_proc = WNDPROC(SetWindowLongPtrA(FindWindowW(L"Valve001", nullptr), GWLP_WNDPROC, LONG_PTR(my_window_proc)));
	
		//�滻present��reset����
		original_present = **reinterpret_cast<decltype(original_present)**>(g_config.memory.present);
		**reinterpret_cast<decltype(my_present)***>(g_config.memory.present) = my_present;
		original_reset = **reinterpret_cast<decltype(original_reset)**>(g_config.memory.reset);
		**reinterpret_cast<decltype(my_reset)***>(g_config.memory.reset) = my_reset;

		client.hook_at(37, frame_stage_notify);
		clientMode.hook_at(24, create_move);
		clientMode.hook_at(44, do_post_screen_effects);

		g_config.gmae_ui->message_box("good message", "csgo helper inject finish");
	}

	//�ָ���Ϸԭ����
	void restore() noexcept
	{
		clientMode.restore();
		client.restore();

		glow_space::clear_custom_objects();

		SetWindowLongPtrA(FindWindowW(L"Valve001", nullptr), GWLP_WNDPROC, LONG_PTR(original_window_proc));
		**reinterpret_cast<void***>(g_config.memory.present) = original_present;
		**reinterpret_cast<void***>(g_config.memory.reset) = original_reset;
	}

	vmt_class clientMode{ g_config.client_mode };//�ͻ�ģʽhook
	vmt_class client{ g_config.client };//�ͻ���hook

	//ԭʼ��Ϸ���ڹ���
	WNDPROC original_window_proc;

	//Present
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> original_present;
	
	//Reset
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> original_reset;
};

extern hooks_class g_hooks;

//����׼���Ĵ��ڹ���
static LRESULT __stdcall my_window_proc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//����imgui����Ĵ��ڹ���
	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//����imgui���������Ϣ����
	ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);

	//����imgui����ʾ������
	if (msg == WM_KEYDOWN && wParam == VK_INSERT) g_config.control.show_imgui = !g_config.control.show_imgui;

	//�ص�ԭʼ����Ϸ���ڵ�ַ
	return CallWindowProc(g_hooks.original_window_proc, window, msg, wParam, lParam);
}

//�����Լ���present����
static HRESULT __stdcall my_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
{
	//��ʼ��imgui��DX9�豸
	static bool imguiInit{ ImGui_ImplDX9_Init(device) };

	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	IDirect3DVertexDeclaration9* vertexDeclaration;
	device->GetVertexDeclaration(&vertexDeclaration);

	g_imgui.render();

	device->SetVertexDeclaration(vertexDeclaration);
	vertexDeclaration->Release();

	return g_hooks.original_present(device, src, dest, windowOverride, dirtyRegion);
}

//�����Լ���reset����
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

	//û������
	if (!cmd->command_number) return state;

	if (g_config.engine->is_in_game())
	{
		aim_space::run(cmd);
		report_space::run();
		other_space::again_jump(cmd);
	}

	return state;
}



