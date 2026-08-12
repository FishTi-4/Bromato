#pragma once

#include "context.h"

class SceneMenu : public Scene
{
	Button btnStart;		//开始游戏
	Button btnSettings;		//设置
	Button btnHistory;		//历史记录
	Button btnAbout;	//开发人员名单
	Button btnExit;			//退出游戏

public:
	SceneMenu();
	void Input() override;
	void Update() override;
	void Draw() override;
};
