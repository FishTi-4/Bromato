#pragma once

#include "ItemManager.h"

enum class Character
{				
	Ox,			// 公牛 
	Doctor,		// 医生
	Fighter,	// 斗士
	Multiskill,	// 多面手
	Allskill,	// 全能
	Count		// 蹩脚英文（）
};

class CharacterManager
{
	// 角色会带来某些增益/减益效果
	// 发现这和道具类似
	// 于是我们把角色视为道具
	Attribute characters[Idx(Character::Count)] =
	{
		{0, 0, 0, 0, 0, 0.15f, 0, 0, 0, TextureName::Ox},
		{0.3f, 0, 0, 0, 0, 0, 0, 0, 0, TextureName::Doctor},
		{0, 0, 0, 0.3f, 0, 0, 0, 0, 0, TextureName::Fighter},
		{0.15f, 0, 0, 0, 0.15f, 0.15f, 0, 0, 0, TextureName::MultiSkill},
		{0.1f, 0, 0.1f, 0.1f, 0.1f, 0.1f, 0, 0, 0, TextureName::AllSkill},
	};

public:
	// 获取一个属性类对象 （道具，地图也都是属性(Attribute)类）
	const Attribute& GetCharacter(const Character& t) const
	{
		return characters[Idx(t)];
	}
};

