#include "config.hpp"
#include "hooks.hpp"
#include "imgui_gui.hpp"

//����������
class hide_self
{
public:
	hide_self(void* module)
	{
		void* pPEB = nullptr;

		//��ȡPEBָ��
		_asm
		{
			push eax
			mov eax, fs:[0x30]
			mov pPEB, eax
			pop eax
		}

		//�����õ�����ȫ��ģ���˫������ͷָ��
		void* pLDR = *((void**)((unsigned char*)pPEB + 0xc));
		void* pCurrent = *((void**)((unsigned char*)pLDR + 0x0c));
		void* pNext = pCurrent;

		//��������б�������ָ��ģ����ж�������
		do
		{
			void* pNextPoint = *((void**)((unsigned char*)pNext));
			void* pLastPoint = *((void**)((unsigned char*)pNext + 0x4));
			void* nBaseAddress = *((void**)((unsigned char*)pNext + 0x18));

			if (nBaseAddress == module)
			{
				*((void**)((unsigned char*)pLastPoint)) = pNextPoint;
				*((void**)((unsigned char*)pNextPoint + 0x4)) = pLastPoint;
				pCurrent = pNextPoint;
			}

			pNext = *((void**)pNext);
		} while (pCurrent != pNext);
	}
};

configuration_struct g_config;//�����ļ�
net_vars_class g_net_vars;//�����Ϣ����
imgui_gui_class g_imgui;//������ʾ
hooks_class g_hooks;//��Ϸ���̽ٳ�

//Ϊ��ģ���ܹ�����ж��
BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	static hide_self init(_DllHandle);
	if (_Reason == DLL_PROCESS_DETACH) g_hooks.restore();
	return TRUE;
}

//---ע���ʼ����˳�򣬲�Ȼ����ָ�ʽ�����Ĵ���---//
