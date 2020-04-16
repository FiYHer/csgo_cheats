#include "config.hpp"
#include "hooks.hpp"
#include "imgui_gui.hpp"

//隐藏自身类
class hide_self
{
public:
	hide_self(void* module)
	{
		void* pPEB = nullptr;

		//读取PEB指针
		_asm
		{
			push eax
			mov eax, fs:[0x30]
			mov pPEB, eax
			pop eax
		}

		//操作得到保存全部模块的双向链表头指针
		void* pLDR = *((void**)((unsigned char*)pPEB + 0xc));
		void* pCurrent = *((void**)((unsigned char*)pLDR + 0x0c));
		void* pNext = pCurrent;

		//对链表进行遍历，对指定模块进行断链隐藏
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

configuration_struct g_config;//配置文件
net_vars_class g_net_vars;//玩家信息辅助
imgui_gui_class g_imgui;//界面显示
hooks_class g_hooks;//游戏流程劫持

//为了模块能够正常卸载
BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	static hide_self init(_DllHandle);
	if (_Reason == DLL_PROCESS_DETACH) g_hooks.restore();
	return TRUE;
}

//---注意初始化的顺序，不然会出现各式各样的错误---//
