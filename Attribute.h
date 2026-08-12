#pragma once

#include "AssetsManager.h"

struct Attribute
{
	float hpMul = 1;			// 最大生命值因子
	float hpAdd = 200;			// 最大生命值增加
	float defenceMul = 1;		// 防御 受到伤害因子
	float damageMul = 1;		// 伤害因子
	float hasteMul = 1;			// 攻速因子
	float speedMul = 1;		// 移速因子
	float dodgeMul = 1;			// 闪避因子
	float critiattackMul = 0.1;	// 暴击率因子
	float critidamageMul = 1;	// 暴击伤害因子
	TextureName textureName;
};

// 关于数值计算：
// 当前文件里存的数值，就是玩家的默认白板数值
// 角色实际最大生命值 = (默认数值 + 所有 道具/角色/地图 提供的 hpAdd) * hpMul
// 角色每次受到的实际伤害 = 伤害 / (默认数值 + 所有 道具/角色/地图 提供的 defenceMul 之和)
// 角色每次造成的实际伤害 = 伤害 * (默认数值 + 所有 道具/角色/地图 提供的 damageMul 之和)
// 武器开火后有冷却时间，每次开火后，实际刷新的冷却时间 = 武器本身的冷却时间 / (默认数值 + 所有 道具/角色/地图 提供的 hasteMul 之和)
// 角色实际移动速度 = 默认移动速度 * (默认数值 + 所有 道具/角色/地图 提供的 speedMul 之和)
// 闪避因子 暴击率 暴击伤害 暂时实现进游戏