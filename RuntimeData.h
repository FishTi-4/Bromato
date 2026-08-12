#pragma once

#include <raylib.h>
#include "WeaponsManager.h"
#include "ItemManager.h"
#include "CharacterManager.h"
#include "MapManager.h"

struct RuntimeData
{
	float dt;				// 帧时间
	Vector2 mouse;			// 逻辑分辨率中的鼠标位置

	Character character;	// 选择的角色
	Map map;				// 选择的地图
	int weaponLevel[Idx(WeaponName::Count)];	// 每种武器的等级
	int itemCount[Idx(ItemName::Count)];		// 每种道具持有数量

	Attribute attribute;	// 属性

	int wave;			// 波次
	int enemyKilled;	// 击毙敌人数
	float timer;		// 计时器
	int coin;			// 金币

	int volume = 100;			
	int fps = 60;
	int difficulty;		// 1 2 3 

	bool isDead;								// 玩家是否存活
	bool waveOver;
	float totalTimer;	// 整局游戏持续时间
};
