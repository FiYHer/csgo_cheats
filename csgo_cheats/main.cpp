#include "config.hpp"
#include "hooks.hpp"
#include "imgui_gui.hpp"

configuration_struct g_config;//�����ļ�
net_vars_class g_net_vars;//�����Ϣ����
imgui_gui_class g_imgui;//������ʾ
hooks_class g_hooks;//��Ϸ���̽ٳ�

//Ϊ��ģ���ܹ�����ж��
BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	if (_Reason == DLL_PROCESS_ATTACH) DisableThreadLibraryCalls((HMODULE)_DllHandle);
	if (_Reason == DLL_PROCESS_DETACH) g_hooks.restore();
	return TRUE;
}

//---ע���ʼ����˳�򣬲�Ȼ����ָ�ʽ�����Ĵ���---//
