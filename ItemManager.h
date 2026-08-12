 #pragma once
#include "AssetsManager.h"
#include "Attribute.h"

enum class ItemName
{
	Healthmul,
	Defence,
	Damage,
	Haste,
	Speed,
	Healthadd,
	Glasses,
	Potato,
	Count,
};

class ItemManager
{
	// 道具可以对玩家属性造成增益/减益
	// 具体有哪些属性，参见 Attribute.h（属性）
	Attribute items[Idx(ItemName::Count)] =
	{
		{0.1f,   0.0f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Heal},		// 生命值 +10%
		{0.0f,   0.0f,  0.1f, 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Helmet},	// 受到伤害 -5%
		{0.0f,   0.0f,  0.0f, 0.1f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Glove},		// 伤害 +10%
		{0.0f,   0.0f,  0.0f, 0.0f,  0.05f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Sausage},	// 攻速 +10%
		{0.0f,   0.0f,  0.0f, 0.0f,  0.0f,  0.1f, 0.0f, 0.0f, 0.0f, TextureName::Wings},		// 移速 +10%
		{0.0f, 100.0f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Plant},	
		{0.05f,   0.0f,  0.05f, 0.00f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Glasses},	
		{0.0f,   0.0f,  0.0f, 0.05f,  0.0f, 0.05f, 0.0f, 0.0f, 0.0f, TextureName::Potato},	
	};
public:
	// 获取一个属性类对象
	const Attribute& GetItem(const ItemName& t) const
	{
		return items[Idx(t)];
	}
};
