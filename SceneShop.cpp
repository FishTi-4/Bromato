#include "SceneShop.h"
#include <utility>
#include <algorithm>

ShopItem::ShopItem(ItemName item, int cost) {
	this->item = item;
	this->cost = cost;
}

SceneShop::SceneShop()
	:Upgrade_Pistol_Button({ 480,200 }, { 300,100 }, "升级")
	, Upgrade_Smg_Button({ 480, 380 }, { 300, 100 }, "升级")
	, Upgrade_Shotgun_Button({ 480, 560 }, { 300, 100 }, "升级")
	, Upgrade_Scout_Button({ 480, 740 }, { 300, 100 }, "升级")
	, Refresh_Button({ 735, 880 }, "刷新")
	, Continue_Button({ 1370, 880 }, "继续")
	, Toggle_ShopView_Button({ 1520, 20 }, { 200, 60 }, "背包")
	, live_time_message(0)
{
	InitWeaponSlots();
	GenerateShopItems();
}

void SceneShop::Input() {}

static void DrawTextureScaled(TextureName name, float x, float y, float w, float h)
{
	const Texture2D& tex = assets.GetTexture(name);
	Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
	Rectangle dst = { x, y, w, h };
	DrawTexturePro(tex, src, dst, { 0, 0 }, 0, WHITE);
}

static std::string GetItemNameStr(ItemName item)
{
	switch (item)
	{
	case ItemName::Healthmul: return "生命倍率";
	case ItemName::Defence:   return "防御";
	case ItemName::Damage:    return "伤害";
	case ItemName::Haste:     return "攻速";
	case ItemName::Speed:     return "移速";
	case ItemName::Healthadd: return "生命加成";
	case ItemName::Glasses:   return "眼镜";
	case ItemName::Potato:    return "土豆";
	default: return "";
	}
}

static std::string GetItemDesc(ItemName item)
{
	const Attribute& a = items.GetItem(item);
	std::string desc;
	if (a.hpMul != 0) desc += TextFormat("最大生命 %+.0f%%", a.hpMul * 100);
	if (a.hpAdd != 0) desc += TextFormat("%s最大生命 %+.0f", desc.empty() ? "" : "，", a.hpAdd);
	if (a.defenceMul != 0) desc += TextFormat("%s防御 %+.0f%%", desc.empty() ? "" : "，", a.defenceMul * 100);
	if (a.damageMul != 0) desc += TextFormat("%s伤害 %+.0f%%", desc.empty() ? "" : "，", a.damageMul * 100);
	if (a.hasteMul != 0) desc += TextFormat("%s攻速 %+.0f%%", desc.empty() ? "" : "，", a.hasteMul * 100);
	if (a.speedMul != 0) desc += TextFormat("%s移速 %+.0f%%", desc.empty() ? "" : "，", a.speedMul * 100);
	return desc;
}

static std::string GetWeaponNameStr(WeaponName w)
{
	switch (w)
	{
	case WeaponName::Pistol: return "手枪";
	case WeaponName::Smg:    return "冲锋枪";
	case WeaponName::Shotgun:return "霰弹枪";
	case WeaponName::Scout:  return "狙击枪";
	default: return "";
	}
}

static std::string GetCharacterNameStr(Character c)
{
	switch (c)
	{
	case Character::Ox:         return "公牛";
	case Character::Doctor:     return "医生";
	case Character::Fighter:    return "斗士";
	case Character::Multiskill: return "多面手";
	case Character::Allskill:   return "全能";
	default: return "";
	}
}

static std::string GetMapNameStr(Map m)
{
	switch (m)
	{
	case Map::Mp1: return "普通";
	case Map::Mp2: return "地牢";
	case Map::Mp3: return "沙漠";
	case Map::Mp4: return "泥潭";
	default: return "";
	}
}

void SceneShop::Draw()
{
	ClearBackground(Color{ 16, 20, 28, 255 });

	DrawCoinDisplay();

	if (current_View == ShopView::Shop)
		DrawShopView();
	else
		DrawBackpackView();

	Continue_Button.Draw();
	Toggle_ShopView_Button.Draw();

	if (!Shop_message.empty())
	{
		int textWidth = MeasureTextEx(fontCN, Shop_message.c_str(), 24, 1).x;
		DrawTextEx(fontCN, Shop_message.c_str(), { (float)(960 - textWidth / 2),900 }, 24, 1, YELLOW);
	}
}

void SceneShop::DrawCoinDisplay()
{
	DrawTextureScaled(TextureName::Coin, 40, 20, 48, 48);

	std::string text = "金币: " + std::to_string(data.coin);
	DrawTextEx(fontCN, text.c_str(), { 100,28 }, 30, 1, GOLD);
}

void SceneShop::DrawShopView()
{
	// ---- 左侧：武器升级 ----
	ClearBackground({59, 45, 7, 255});
	DrawTextEx(fontCN, "=== 武器升级 ===", { 40,100 }, 36, 1, WHITE);

	// 手枪
	const Weapon& pistolData = weapons.GetWeapon(WeaponName::Pistol);
	DrawTextureScaled(pistolData.textureName, 60, 160, 64, 64);
	DrawTextEx(fontCN, "手枪", { 140,165 }, 28, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("等级: %d", data.weaponLevel[Idx(WeaponName::Pistol)]), { 140,200 }, 22, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("伤害: %.1f", pistolData.damage + pistolData.upgradeDamage * Pistol.currentLevel), { 140,230 }, 20, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("升级费用: %d", Pistol.upgradeCost), { 140,260 }, 20, 1, GOLD);
	Upgrade_Pistol_Button.Draw();

	// SMG
	const Weapon& smgData = weapons.GetWeapon(WeaponName::Smg);
	DrawTextureScaled(smgData.textureName, 60, 340, 64, 64);
	DrawTextEx(fontCN, "冲锋枪", { 140,345 }, 28, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("等级: %d", data.weaponLevel[Idx(WeaponName::Smg)]), { 140,380 }, 22, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("伤害: %.1f", smgData.damage + smgData.upgradeDamage * Smg.currentLevel), { 140,410 }, 20, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("升级费用: %d", Smg.upgradeCost), { 140,440 }, 20, 1, GOLD);
	Upgrade_Smg_Button.Draw();

	// Shotgun
	const Weapon& shotgunData = weapons.GetWeapon(WeaponName::Shotgun);
	DrawTextureScaled(shotgunData.textureName, 60, 520, 64, 64);
	DrawTextEx(fontCN, "霰弹枪", { 140,525 }, 28, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("等级: %d", data.weaponLevel[Idx(WeaponName::Shotgun)]), { 140,560 }, 22, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("伤害: %.1f", shotgunData.damage + shotgunData.upgradeDamage * Shotgun.currentLevel), { 140,590 }, 20, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("升级费用: %d", Shotgun.upgradeCost), { 140,620 }, 20, 1, GOLD);
	Upgrade_Shotgun_Button.Draw();

	// Scout
	const Weapon& scoutData = weapons.GetWeapon(WeaponName::Scout);
	DrawTextureScaled(scoutData.textureName, 60, 700, 64, 64);
	DrawTextEx(fontCN, "狙击枪", { 140,705 }, 28, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("等级: %d", data.weaponLevel[Idx(WeaponName::Scout)]), { 140,740 }, 22, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("伤害: %.1f", scoutData.damage + scoutData.upgradeDamage * Scout.currentLevel), { 140,770 }, 20, 1, WHITE);
	DrawTextEx(fontCN, TextFormat("升级费用: %d", Scout.upgradeCost), { 140,800 }, 20, 1, GOLD);
	Upgrade_Scout_Button.Draw();

	// ---- 右侧：道具购买 ----
	DrawTextEx(fontCN, "=== 道具购买 ===", { 980,100 }, 36, 1, WHITE);

	for (int i = 0; i < Items_displayed.size(); i++)
	{
		const ShopItem& entry = Items_displayed[i];
		float y = 175.0f + i * 160.0f;

		// 图标
		DrawTextureScaled(items.GetItem(entry.item).textureName, 1000, y, 48, 48);

		// 名称 + 持有数
		DrawTextEx(fontCN, TextFormat("%s x%d", GetItemNameStr(entry.item).c_str(), data.itemCount[Idx(entry.item)]), {1060,y + 5}, 24, 1, WHITE);

		// 属性描述
		DrawTextEx(fontCN, GetItemDesc(entry.item).c_str(), { 1060,y + 35 }, 18, 1, GRAY);

		// 价格
		DrawTextEx(fontCN, TextFormat("价格: %d", entry.cost), { 1060,y + 58 }, 18, 1, GOLD);

		// 购买按钮（已售出的灰色跳过）
		if (Item_Button_status[i])
			BuyItems_Button[i].Draw();
	}

	// ---- 底部：刷新按钮 ----
	Refresh_Button.Draw();
}

void SceneShop::DrawBackpackView()
{
	DrawTextEx(fontCN, "=== 背包 ===", { 960 - MeasureTextEx(fontCN,"=== 背包 ===",40,1).x / 2,40 }, 40, 1, WHITE);

	// ---- 角色 ----
	DrawTextEx(fontCN, "角色:", { 60,120 }, 28, 1, WHITE);
	DrawTextureScaled(characters.GetCharacter(data.character).textureName, 60, 160, 80, 80);
	DrawTextEx(fontCN, GetCharacterNameStr(data.character).c_str(), { 160,185 }, 28, 1, WHITE);

	// ---- 地图 ----
	DrawTextEx(fontCN, "地图:", { 500,120 }, 28, 1, WHITE);
	DrawTextureScaled(maps.GetMap(data.map).textureName, 500, 160, 80, 80);
	DrawTextEx(fontCN, GetMapNameStr(data.map).c_str(), { 600,185 }, 28, 1, WHITE);

	// ---- 当前属性 ----
	DrawTextEx(fontCN, "当前属性:", { 1030,155 }, 30, 1, Color{ 98,216,155,255 });

	float maxHP = data.attribute.hpAdd * data.attribute.hpMul;
	DrawTextEx(fontCN, "最大生命", { 1030,220 }, 26, 1, LIGHTGRAY);
	DrawTextEx(fontCN, TextFormat("%.0f",maxHP), { 1450,220 }, 26, 1, WHITE);
	DrawTextEx(fontCN, "防御", { 1030,275 }, 26, 1, LIGHTGRAY);
	DrawTextEx(fontCN, TextFormat("x%.2f",data.attribute.defenceMul), { 1450,275 }, 26, 1, WHITE);
	DrawTextEx(fontCN, "伤害", { 1030,330 }, 26, 1, LIGHTGRAY);
	DrawTextEx(fontCN, TextFormat("x%.2f",data.attribute.damageMul), { 1450,330 }, 26, 1, WHITE);
	DrawTextEx(fontCN, "攻速", { 1030,385 }, 26, 1, LIGHTGRAY);
	DrawTextEx(fontCN, TextFormat("x%.2f",data.attribute.hasteMul), { 1450,385 }, 26, 1, WHITE);
	DrawTextEx(fontCN, "移速", { 1030,440 }, 26, 1, LIGHTGRAY);
	DrawTextEx(fontCN, TextFormat("x%.2f",data.attribute.speedMul), { 1450,440 }, 26, 1, WHITE);

	// ---- 武器 ----
	DrawTextEx(fontCN, "武器:", { 60,300 }, 28, 1, WHITE);

	for (int i = 0; i < Idx(WeaponName::Count); i++)
	{
		WeaponName weapon = (WeaponName)i;
		float y = 350.0f + i * 60.0f;
		DrawTextureScaled(weapons.GetWeapon(weapon).textureName, 60, y, 48, 48);
		DrawTextEx(fontCN, TextFormat("%s  等级%d", GetWeaponNameStr(weapon).c_str(), data.weaponLevel[Idx(weapon)]), {120,y + 10}, 24, 1, WHITE);
	}

	// ---- 道具 ----
	DrawTextEx(fontCN, "道具:", { 60,620 }, 28, 1, WHITE);

	for (int i = 0; i < Idx(ItemName::Count); i++)
	{
		ItemName item = (ItemName)i;
		float x = 60.0f + i % 4 * 180.0f;
		float y = 670.0f + i / 4 * 130.0f;
		int count = data.itemCount[Idx(item)];
		Color c = (count > 0) ? WHITE : GRAY;

		DrawTextureScaled(items.GetItem(item).textureName, x, y, 64, 64);
		DrawTextEx(fontCN, GetItemNameStr(item).c_str(), { x,y + 68 }, 18, 1, c);
		DrawTextEx(fontCN, TextFormat("x%d", count), { x + 10,y + 92 }, 20, 1, c);
	}
}

static std::string successmessage = "操作成功";
static std::string fatelmessage = "金币不足";
void SceneShop::Update()
{
	audio.Update();
	if (Toggle_ShopView_Button.Update()) {
		audio.PlaySFX(SFXType::Push);
		if (current_View == ShopView::Shop) {
			current_View = ShopView::Backpack;
			Toggle_ShopView_Button.SetText("商店");
		}
		else {
			current_View = ShopView::Shop;
			Toggle_ShopView_Button.SetText("背包");
		}
	}

	if (current_View == ShopView::Shop) {
		if (Upgrade_Pistol_Button.Update()) {
			if (data.coin >= Pistol.upgradeCost) {
				data.coin -= Pistol.upgradeCost;
				data.weaponLevel[Idx(WeaponName::Pistol)]++;
				AddMessage(successmessage);
				audio.PlaySFX(SFXType::Buy);
				InitWeaponSlots();
			}
			else {
				AddMessage(fatelmessage);
			}
		}

		if (Upgrade_Smg_Button.Update()) {
			if (data.coin >= Smg.upgradeCost) {
				data.coin -= Smg.upgradeCost;
				data.weaponLevel[Idx(WeaponName::Smg)]++;
				AddMessage(successmessage);
				audio.PlaySFX(SFXType::Buy);
				InitWeaponSlots();
			}
			else {
				AddMessage(fatelmessage);
			}
		}

		if (Upgrade_Shotgun_Button.Update()) {
			if (data.coin >= Shotgun.upgradeCost) {
				data.coin -= Shotgun.upgradeCost;
				data.weaponLevel[Idx(WeaponName::Shotgun)]++;
				AddMessage(successmessage);
				audio.PlaySFX(SFXType::Buy);
				InitWeaponSlots();
			}
			else {
				AddMessage(fatelmessage);
			}
		}

		if (Upgrade_Scout_Button.Update()) {
			if (data.coin >= Scout.upgradeCost) {
				data.coin -= Scout.upgradeCost;
				data.weaponLevel[Idx(WeaponName::Scout)]++;
				AddMessage(successmessage);
				audio.PlaySFX(SFXType::Buy);
				InitWeaponSlots();
			}
			else {
				AddMessage(fatelmessage);
			}
		}

		for (int i = 0; i < BuyItems_Button.size(); i++) {
			if (!Item_Button_status[i]) {
				continue;
			}
			if (BuyItems_Button[i].Update()) {
				ShopItem& entry = Items_displayed[i];
				if (data.coin >= entry.cost) {
					data.coin -= entry.cost;
					data.itemCount[Idx(entry.item)]++;
					ApplyAttribute(items.GetItem(entry.item));
					AddMessage(successmessage);
					audio.PlaySFX(SFXType::Buy);
					Item_Button_status[i] = false;
					break;
				}
				else {
					AddMessage(fatelmessage);
				}
			}
		}

		if (Refresh_Button.Update()) {
			if (data.coin >= Refresh_Cost) {
				data.coin -= Refresh_Cost;
				GenerateShopItems();
				AddMessage(successmessage);
				audio.PlaySFX(SFXType::Buy);
			}
			else {
				AddMessage(fatelmessage);
			}
		}
	}

	if (Continue_Button.Update()) {
		SetNextScene(SceneType::Game);
		audio.PlaySFX(SFXType::Push);
	}

	if (live_time_message > 0) {
		live_time_message -= data.dt;
		if (live_time_message <= 0) {
			Shop_message = "";
		}
	}
}

void SceneShop::InitWeaponSlots() {
	Pistol.currentLevel = data.weaponLevel[Idx(WeaponName::Pistol)];
	Pistol.upgradeCost = Weapon_base_Cost;
	if (Pistol.currentLevel >= Weapon_Level_Max) {
		Upgrade_Pistol_Button.Disable();
		Upgrade_Pistol_Button.SetText("已满级");
	}
	else {
		Upgrade_Pistol_Button.Enable();
		Upgrade_Pistol_Button.SetText("升级");
	}

	Smg.currentLevel = data.weaponLevel[Idx(WeaponName::Smg)];
	Smg.upgradeCost = Weapon_base_Cost;
	if (Smg.currentLevel >= Weapon_Level_Max) {
		Upgrade_Smg_Button.Disable();
		Upgrade_Smg_Button.SetText("已满级");
	}
	else {
		Upgrade_Smg_Button.Enable();
		Upgrade_Smg_Button.SetText("升级");
	}

	Shotgun.currentLevel = data.weaponLevel[Idx(WeaponName::Shotgun)];
	Shotgun.upgradeCost = Weapon_base_Cost;
	if (Shotgun.currentLevel >= Weapon_Level_Max) {
		Upgrade_Shotgun_Button.Disable();
		Upgrade_Shotgun_Button.SetText("已满级");
	}
	else {
		Upgrade_Shotgun_Button.Enable();
		Upgrade_Shotgun_Button.SetText("升级");
	}

	Scout.currentLevel = data.weaponLevel[Idx(WeaponName::Scout)];
	Scout.upgradeCost = Weapon_base_Cost;
	if (Scout.currentLevel >= Weapon_Level_Max) {
		Upgrade_Scout_Button.Disable();
		Upgrade_Scout_Button.SetText("已满级");
	}
	else {
		Upgrade_Scout_Button.Enable();
		Upgrade_Scout_Button.SetText("升级");
	}
}

void SceneShop::GenerateShopItems() {
	Items_displayed.clear();
	BuyItems_Button.clear();
	Item_Button_status.clear();

	std::vector<std::pair<int, ItemName> >pool = {
		{0,ItemName::Healthmul},
		{0,ItemName::Defence},
		{0,ItemName::Damage} ,
		{0,ItemName::Haste},
		{0,ItemName::Speed},
		{0,ItemName::Healthadd},
		{0,ItemName::Glasses},
		{0,ItemName::Potato}
	};

	for (int i = 0; i < Idx(ItemName::Count); i++) {
		pool[i].first = GetRandomValue(0, 100);
	}
	std::sort(pool.begin(), pool.end());

	for (int i = 0; i < Max_ShopItem; i++) {
		ItemName item = pool[i].second;
		int Cost = Item_base_Cost;
		Items_displayed.push_back(std::move(ShopItem(item, Cost)));

		float y = 175.0 + i * 160.0;
		BuyItems_Button.push_back(std::move(Button({ 1300, y }, { 250, 80 }, "购买")));
		Item_Button_status.push_back(true);
	}
}

void SceneShop::AddMessage(std::string msg) {
	Shop_message = msg;
	live_time_message = 2;
}

void SceneShop::ApplyAttribute(const Attribute& src) {
	data.attribute.hpAdd += src.hpAdd;
	data.attribute.speedMul += src.speedMul;
	data.attribute.hpMul += src.hpMul;
	data.attribute.defenceMul += src.defenceMul;
	data.attribute.damageMul += src.damageMul;
	data.attribute.hasteMul += src.hasteMul;
	data.attribute.dodgeMul += src.dodgeMul;
	data.attribute.critiattackMul += src.critiattackMul;
	data.attribute.critidamageMul += src.critidamageMul;
}
