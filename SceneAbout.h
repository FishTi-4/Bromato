#pragma once

#include "context.h"
#include <vector>
#include <string>

struct Credit
{
	std::string name;//名字
	std::string role;//职责
};

class SceneAbout : public Scene
{
	std::vector<Credit>credits;
	std::string version;
	Button backButton;//返回
	float currentroll = 0;//当前滚动
	float targetroll = 0;//目标滚动
	float contentH = 0;//总高

public:
	SceneAbout();
	void Input() override;
	void Update() override;
	void Draw() override;
};
