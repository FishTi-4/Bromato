#pragma once

#include "raylib.h"

extern Font fontCN;

class ChineseFontHandler
{
public:
	void Load()
	{
		const char* text =
			"开始游戏设置历史记录关于我们退出游戏暂停继续返回"
			"生命值防御伤害攻速移速金币等级"
			"公牛医生斗士多面手全能"
			"商店购买刷新游戏结束胜利失败"
			"地图武器角色道具玩家敌人"
			"本局数据最终属性到达波次生存时间击败持有金币最大生命"
			"手枪冲锋枪霰弹枪狙击枪查看历史按回车或退出键秒未知"
			"一二三四五六七八九十"
			"！？。，：；（）"
			"0123456789.x% "
			"历史记录存档列表暂无存档记录按 ↑/↓ 键选择按 ESC 返回主菜单选中存档详情"
			"波次击杀金币角色地图难度武器道具请从左侧选择存档加载保存预览"
			"手枪冲锋枪霰弹枪狙击枪未知武器生命倍率防御伤害急速移速加成眼镜土豆未知道具"
			"公牛医生斗士多面手全能未知角色第一关第二关第三关第四关未知地图普通选择角色和地图"
			"1234567890Lv. x:："
			"组长产品经理信息官技术官咸鱼夜游呆猫蒟蒻黑蛋雪梨副监督官版本号开发引擎帧率制作人员分辨"
			"+-/*BromatocFPSCvraylibv返回"
			" !\"\#\$ \% \&'\(\)*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			"返回主菜单暴击率闪避""默认""地牢""沙漠""泥潭武器地图角色背包价格=== 道具购买 ===伤害升级费用等级" 
			"结束游戏剩余时间游戏已暂停开始音量帧率限制拖动滑条修改数值最大受到攻击速度移动操作成功不足商店已满级"
			"存档次游戏最近的第全屏当前"
			
			
			
			
			;
		int count = 0;			
		int* codepoints = LoadCodepoints(text, &count);
		fontCN = LoadFontEx("assets/fonts/simhei.ttf",64,codepoints,count);
		UnloadCodepoints(codepoints);
		SetTextureFilter(fontCN.texture,TEXTURE_FILTER_BILINEAR);
	}

	void Unload() { UnloadFont(fontCN); }
};


/*		- 绘制中文只需 DrawtextEx 中使用字体 fontCN 即可
		- 要用的中文字符需要添加到 context 筛选器中的 ChineseFontHandler.h 中*/
