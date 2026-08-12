#pragma once

#include "context.h"
#include <vector>

enum class ShopView {
	Shop,
	Backpack
};

class ShopItem {
public:
	ShopItem(ItemName item, int cost);
	ItemName item;
	int cost;
};

class ShopWeapon {
public:
	WeaponName weapon;
	int currentLevel;
	int upgradeCost;
};

class SceneShop : public Scene
{
public:
	SceneShop();
	ShopView current_View = ShopView::Shop;

	Button Upgrade_Pistol_Button;
	Button Upgrade_Smg_Button;
	Button Upgrade_Shotgun_Button;
	Button Upgrade_Scout_Button;
	ShopWeapon Pistol;
	ShopWeapon Smg;
	ShopWeapon Shotgun;
	ShopWeapon Scout;

	std::vector<ShopItem> Items_displayed;
	std::vector<Button> BuyItems_Button;
	std::vector<bool> Item_Button_status;

	Button Refresh_Button;
	Button Continue_Button;
	Button Toggle_ShopView_Button;

	std::string Shop_message;
	double live_time_message;

	void Input() override;
	void Update() override;
	void Draw() override;
private:
	int Refresh_Cost = 100;
	int Max_ShopItem = 4;
	int Weapon_base_Cost = 100;
	int Item_base_Cost = 100;
	int Weapon_Level_Max = INT_MAX;

	void InitWeaponSlots();
	void GenerateShopItems();
	void AddMessage(std::string msg);
	void ApplyAttribute(const Attribute& src);
	void DrawShopView();
	void DrawBackpackView();
	void DrawCoinDisplay();
};

