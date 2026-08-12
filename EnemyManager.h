#pragma once

#include "Enemy.h"

class EnemyManager
{
private:
	// 敌人管理器中存放从 Entity（实体）父类继承下来的 Enemy 子类
	// 敌人的配置内容如下：（只有 Ranged 是远程敌人，会发射子弹）
	// 
	// 
	// 敌人类型名（使用 Enemy.h 中的枚举）	
	// 速度
	// 敌人的半径（不是敌人的攻击半径）
	// 敌人的生命值	
	// 接触伤害（玩家碰到敌人掉血量）
	// 子弹伤害（Ranged 发射的子弹的伤害）
	// 子弹速度（Ranged 发射的子弹的速度）
	// 子弹半径（Ranged 发射的子弹的半径）
	// 射击间隔（Ranged 发射子弹的时间间隔）
	// 敌人的材质
	Enemy enemies[Idx(EnemyName::Count)] =
	{
		{ EnemyName::Normal1, 250,30,150,25, 0,  0, 0,  0,TextureName::Slow },
		{ EnemyName::Normal2, 350,30,80,25, 0,  0, 0,  0,TextureName::Fast },
		{ EnemyName::Ranged,    30,30,40,5,30,430,13,3,TextureName::Remote },
		{ EnemyName::Charge,  200,50,350,50, 0,  0, 0,  0,TextureName::Charge },
		{ EnemyName::Boss,     180,60,1000,100,0,0,0,0,TextureName::Boss},	
	};

public:
	// 获取一个 Enemy 类对象
	const Enemy& GetEnemy(EnemyName t) const { return enemies[Idx(t)]; }
};
