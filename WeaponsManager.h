#pragma once

#include "AssetsManager.h"

enum class WeaponName
{
	Pistol,				// 手枪
	Smg,				// 冲锋枪
	Shotgun,			// 霰弹枪
	Scout,				// 狙击枪
	Count,
};

// 所有的武器出生就都带在身上
// 在商店可以给武器升级，具体的效果是增加武器的伤害

struct Weapon
{
	float damage;				// 武器伤害
	float upgradeDamage;		// 武器升一级 伤害的增量
	float cooldown;				// 武器攻击冷却时间 单位秒
	float radius;				// 武器攻击半径 敌人出现在半径里才会攻击
	float bulletRadius;			// 子弹半径
	float bulletSpeed;			// 子弹速度
	TextureName textureName;	// 武器材质名
	TextureName bulletTextureName;	// 子弹材质名
};

class WeaponsManager
{
	Weapon weapons[Idx(WeaponName::Count)] =
	{
		// { 伤害, 升级伤害, 冷却, 攻击范围, 子弹半径, 子弹速度, 材质 }
		{25,5,0.43,400,12,550,TextureName::Pistol,TextureName::Bulletw},
		{5,1,0.15,250,9,350,TextureName::Smg,TextureName::Bullety},
		{25,5,1.5,200,20,200,TextureName::Shotgun,TextureName::Bulletb},
		{200,40,3,650,15,2000,TextureName::Scout,TextureName::Bulletp}
	};
public:
	const Weapon& GetWeapon(const WeaponName& t) const { return weapons[Idx(t)]; }
	// 获取一个武器类对象
};
