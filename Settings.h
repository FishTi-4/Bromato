#pragma once

#include "context.h"
#include "BarOptions.h"
#include "RuntimeData.h"

extern RuntimeData data;

class SceneSettings : public Scene
{
public:
	SceneSettings();
	void Input() override;
	void Update() override;
	void Draw() override;

private:
	std::vector<BarOptions> button_optings;
	TextButton<void()> backButton;
	TextButton<void()> fullWindows;
};
