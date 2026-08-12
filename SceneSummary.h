#pragma once

#include "context.h"

class SceneSummary : public Scene
{
private:
	Button backToMenu{ { 735,850 } };

	void DrawCenteredText(const char* text, int y, int fontSize, Color color);

public:
	void Input() override;
	void Update() override;
	void Draw() override;
};
