#pragma once

#include "ItemManager.h"

enum class Map
{
	Mp1,
	Mp2,
	Mp3,
	Mp4,
	Count
};

class MapManager
{
	// 地图会带来某些增益/减益效果
	// 发现这和道具类似
	// 于是我们把地图视为道具

	// 最大生命值因子、最大生命值增加、防御受到伤害因子、伤害因子、攻速因子、移速因子、闪避因子、暴击率因子、暴击伤害因子
	Attribute maps[Idx(Map::Count)] =
	{ //生命因子  生命增加  防御 伤害  攻速   移速   闪避  暴击率  暴击伤害
		
	  // 普通
		{0.5f,   0.0f,  0.0f, 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Mp1},

		// map2：伤害提升，但防御降低
		{0.0f,   0.0f, 0.1f, 0.3f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, TextureName::Mp2},

		// 沙漠：攻速提升，但移速降低
		{0.0f,   0.0f,  0.0f, 0.0f,  0.2f, 0.2f, 0.0f, 0.0f, 0.0f, TextureName::Mp3},

		// mp4：伤害、攻速、移速提升，但更容易受伤
		{0.0f,   0.0f, 0.1f, 0.1f,  0.1f,  0.1f, 0.0f, 0.0f, 0.0f, TextureName::Mp4},
	};
public:
	// 获取一个属性类对象（道具，角色也都是属性(Attribute)类）
	const Attribute& GetMap(const Map& t) const
	{
		return maps[Idx(t)];
	}
};
