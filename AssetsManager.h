#pragma once

#include <raylib.h>

#define Idx(x) (static_cast<int>(x)) // 严格枚举换为下标

// 游戏中所有可使用的材质都有独一无二的资源名（见下方）
// 使用或新增材质的方式见下

enum class TextureName
{
    // Enemy
    Boss, Charge, Fast, Slow, Remote,
    // Player
    AllSkill, Ox, Doctor, Fighter, MultiSkill,
    // Weapon   
    Axe, Knife, Pistol, Smg, Shotgun, Scout,
    // Item
    Glasses, Glove, Heal, Helmet, Plant, Potato, Sausage, Scope, Ghost, Wings,
    // Background
    Bg1, Bg2, Bg3, Charactor, Logo, Logo_new,
    // Map
    Mp1, Mp2, Mp3, Mp4,
    // Other
    Coin, Foot, Bullet, BulletRifle, BulletShotgun, BulletSniper, BulletSubmachine, RedMark, Tree, TreeMark, Upgrade, Bullety, Bulletb, Bulletp, Bulletw,
    // UI
    ButtonNormal, ButtonHovered, ButtonPressed, ButtonDisabled,

    Count
};

//enum class SoundName
//{
//    Attack, Bg, Hit, Push, Shoot,
//
//    Count
//};

class AssetsManager
{
	Texture2D textures[Idx(TextureName::Count)];
	//Sound sounds[Idx(SoundName::Count)];
public:
	void Load();
	void Unload();
	const Texture2D& GetTexture(TextureName);
	//const Sound& GetSound(SoundName);
};

// 资源管理器

// 资源使用方法：
// 全局对象 assets.GetTexture(TextureName::材质名)
// 返回一个 raylib 中的 Texture2D 类

// 添加资源：
// 1. 把要添加的资源放到项目目录 assets/texture/... 对应的文件夹中
// 2. 给这个资源起一个名字，首字母大写，把这个名字添加到上方的枚举中
// 3. 在 AssetsManager.cpp 中添加这个资源的加载代码
// 4. 告知技术官，便于合并代码
