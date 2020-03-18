#include "config.hpp"
#include "hooks.hpp"
#include "imgui_gui.hpp"

configuration_struct g_config;//配置文件
net_vars_class g_net_vars;//玩家信息辅助
imgui_gui_class g_imgui;//界面显示
hooks_class g_hooks;//游戏流程劫持

//为了模块能够正常卸载
BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	if (_Reason == DLL_PROCESS_ATTACH) DisableThreadLibraryCalls((HMODULE)_DllHandle);
	if (_Reason == DLL_PROCESS_DETACH) g_hooks.restore();
	return TRUE;
}

//---注意初始化的顺序，不然会出现各式各样的错误---//
