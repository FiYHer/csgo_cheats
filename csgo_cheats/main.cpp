#include "config.hpp"
#include "hooks.hpp"
#include "imgui_gui.hpp"

configuration_struct g_config;//配置文件
net_vars_class g_net_vars;//玩家信息辅助
imgui_gui_class g_imgui;//界面显示
hooks_class g_hooks;//游戏流程劫持


//---注意初始化的顺序，不然会出现各式各样的错误---//
