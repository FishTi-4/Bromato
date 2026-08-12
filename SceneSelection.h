#pragma once
#include "context.h"

//main中加了一个case跳转
//SceneMenu.cpp里input跳转改了一下Selection
//attribute加了struct mapattribute来单描述地图效果，mapmanager对应改动
//仅这三处额外改动

class SceneSelection : public Scene
{
	int selectedChar = -1;//当前角色编号
	int selectedMap = -1;//当前地图编号

	Button startButton;//开始
	Button backButton;//返回

	Rectangle GetCharRect(int i) const;//第i张角色卡片位置
	Rectangle GetMapRect(int i) const;//第i张地图卡片位置


public:
	SceneSelection();
	void Input() override;
	void Update() override {};
	void Draw() override;
};
