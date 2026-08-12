#include "AssetsManager.h"

void AssetsManager::Load()
{
	// -------------------------------- 材质--------------------------------------
	// 敌人
	textures[Idx(TextureName::Boss)] = LoadTexture("assets/textures/enemy/boss.png");
	textures[Idx(TextureName::Charge)] = LoadTexture("assets/textures/enemy/charge.png");
	textures[Idx(TextureName::Fast)] = LoadTexture("assets/textures/enemy/fast.png");
	textures[Idx(TextureName::Slow)] = LoadTexture("assets/textures/enemy/slow.png");
	textures[Idx(TextureName::Remote)] = LoadTexture("assets/textures/enemy/remote.png");
	// 自机
	textures[Idx(TextureName::AllSkill)] = LoadTexture("assets/textures/player/allskill.png");
	textures[Idx(TextureName::Ox)] = LoadTexture("assets/textures/player/ox.png");
	textures[Idx(TextureName::Doctor)] = LoadTexture("assets/textures/player/doctor.png");
	textures[Idx(TextureName::Fighter)] = LoadTexture("assets/textures/player/fighter.png");
	textures[Idx(TextureName::MultiSkill)] = LoadTexture("assets/textures/player/multiskill.png");
	// 武器
	textures[Idx(TextureName::Axe)] = LoadTexture("assets/textures/weapon/axe.png");
	textures[Idx(TextureName::Knife)] = LoadTexture("assets/textures/weapon/knife.png");
	textures[Idx(TextureName::Pistol)] = LoadTexture("assets/textures/weapon/pistol.png");
	textures[Idx(TextureName::Smg)] = LoadTexture("assets/textures/weapon/smg.png");
	textures[Idx(TextureName::Scout)] = LoadTexture("assets/textures/weapon/scout.png");
	textures[Idx(TextureName::Shotgun)] = LoadTexture("assets/textures/weapon/shotgun.png");
	// 物品
	textures[Idx(TextureName::Glasses)] = LoadTexture("assets/textures/item/glasses.png");
	textures[Idx(TextureName::Glove)] = LoadTexture("assets/textures/item/glove.png");
	textures[Idx(TextureName::Heal)] = LoadTexture("assets/textures/item/heal.png");
	textures[Idx(TextureName::Helmet)] = LoadTexture("assets/textures/item/helmet.png");
	textures[Idx(TextureName::Plant)] = LoadTexture("assets/textures/item/plant.png");
	textures[Idx(TextureName::Potato)] = LoadTexture("assets/textures/item/potato.png");
	textures[Idx(TextureName::Sausage)] = LoadTexture("assets/textures/item/sausage.png");
	textures[Idx(TextureName::Scope)] = LoadTexture("assets/textures/item/scope.png");
	textures[Idx(TextureName::Ghost)] = LoadTexture("assets/textures/item/ghost.png");
	textures[Idx(TextureName::Wings)] = LoadTexture("assets/textures/item/wings.png");
	// 背景
	textures[Idx(TextureName::Bg1)] = LoadTexture("assets/textures/background/bg1.png");
	textures[Idx(TextureName::Bg2)] = LoadTexture("assets/textures/background/bg2.png");
	textures[Idx(TextureName::Bg3)] = LoadTexture("assets/textures/background/bg3.png");
	textures[Idx(TextureName::Charactor)] = LoadTexture("assets/textures/background/charactor.png");
	textures[Idx(TextureName::Logo)] = LoadTexture("assets/textures/background/logo.png");
	textures[Idx(TextureName::Logo_new)] = LoadTexture("assets/textures/background/logo_new.png");
	// 地图
	textures[Idx(TextureName::Mp1)] = LoadTexture("assets/textures/map/mp1.png");
	textures[Idx(TextureName::Mp2)] = LoadTexture("assets/textures/map/mp2.png");
	textures[Idx(TextureName::Mp3)] = LoadTexture("assets/textures/map/mp3.png");
	textures[Idx(TextureName::Mp4)] = LoadTexture("assets/textures/map/mp4.png");
	// 其他
	textures[Idx(TextureName::Coin)] = LoadTexture("assets/textures/other/coin.png");
	textures[Idx(TextureName::Foot)] = LoadTexture("assets/textures/other/foot.png");
	textures[Idx(TextureName::Bullet)] = LoadTexture("assets/textures/other/bullet.png");
	textures[Idx(TextureName::Bulletw)] = LoadTexture("assets/textures/other/bulletw.png");
	textures[Idx(TextureName::Bulletb)] = LoadTexture("assets/textures/other/bulletb.png");
	textures[Idx(TextureName::Bullety)] = LoadTexture("assets/textures/other/bullety.png");
	textures[Idx(TextureName::Bulletp)] = LoadTexture("assets/textures/other/bulletp.png");
	textures[Idx(TextureName::BulletRifle)] = LoadTexture("assets/textures/other/bullet_rifle.png");
	textures[Idx(TextureName::BulletShotgun)] = LoadTexture("assets/textures/other/bullet_shotgun.png");
	textures[Idx(TextureName::BulletSniper)] = LoadTexture("assets/textures/other/bullet_sniper.png");
	textures[Idx(TextureName::BulletSubmachine)] = LoadTexture("assets/textures/other/bullet_submachine.png");
	textures[Idx(TextureName::RedMark)] = LoadTexture("assets/textures/other/redmark.png");
	textures[Idx(TextureName::Tree)] = LoadTexture("assets/textures/other/tree.png");
	textures[Idx(TextureName::TreeMark)] = LoadTexture("assets/textures/other/treemark.png");
	textures[Idx(TextureName::Upgrade)] = LoadTexture("assets/textures/other/upgrade.png");
	// UI
	textures[Idx(TextureName::ButtonNormal)] = LoadTexture("assets/textures/ui/button-normal.png");
	textures[Idx(TextureName::ButtonHovered)] = LoadTexture("assets/textures/ui/button-hovered.png");
	textures[Idx(TextureName::ButtonPressed)] = LoadTexture("assets/textures/ui/button-pressed.png");
	textures[Idx(TextureName::ButtonDisabled)] = LoadTexture("assets/textures/ui/button-disabled.png");
	// -------------------------------- 音效 --------------------------------------
	//sounds[Idx(SoundName::Attack)] = LoadSound("assets/sounds/attack.wav");
	//sounds[Idx(SoundName::Bg)] = LoadSound("assets/sounds/bg.wav");
	//sounds[Idx(SoundName::Hit)] = LoadSound("assets/sounds/hit.wav");
	//sounds[Idx(SoundName::Push)] = LoadSound("assets/sounds/push.wav");
	//sounds[Idx(SoundName::Shoot)] = LoadSound("assets/sounds/shoot.wav");
}

void AssetsManager::Unload()
{
	for (int i = 0; i < Idx(TextureName::Count); i++) UnloadTexture(textures[i]);
	//for (int i = 0; i < Idx(SoundName::Count); i++) UnloadSound(sounds[i]);
}

const Texture2D& AssetsManager::GetTexture(TextureName t)
{
	return textures[Idx(t)];
}

//const Sound& AssetsManager::GetSound(SoundName t)
//{	
//	return sounds[Idx(t)];
//}
