#include "SceneAbout.h"

SceneAbout::SceneAbout() : backButton({ 50, 50 }, 0.5f, "返回")
{
	credits = {
		{"咸鱼","组长/信息官"},
		{"雪梨","副组长/产品经理"},
		{"夜游","技术官"},
		{"呆猫","监督官"},
		{"黑蛋","产品经理/信息官"},
		{"蒟蒻","技术官"}
	};
	//版本
	version = "v1.0.0";

	contentH = 120.0f                     // 标题
		+ 100.0f                     // 副标题
		+ credits.size() * 80.0f     // 人员列表
		+ 40.0f                      // 列表下方间距
		+ 60.0f                      // 分割线区域
		+ 90.0f                      // "游戏信息"标题
		+ 4 * 55.0f                  //  游戏信息
		+ 80.0f;                // 信息下方间距
}

void SceneAbout::Input()
{
	if (backButton.Update())
		SetNextScene(SceneType::Menu), audio.PlaySFX(SFXType::Push);
	float wheel = GetMouseWheelMove();//神秘滚轮
	targetroll -= wheel * 60.0f;
}

void SceneAbout::Update()
{
	float maxroll = fmaxf(0, contentH - 1080);
	targetroll = Clamp(targetroll, 0, maxroll);
	currentroll = Lerp(currentroll, targetroll, 10.f * data.dt);//神秘滑动
}

void SceneAbout::Draw()
{
	ClearBackground(Color{ 16, 20, 28, 255 });
	const int centerx = 960;
	float y = 100 - currentroll;

	//制作人员
	const char* title = "制作人员";
	auto titleWidth = MeasureTextEx(fontCN, title, 70, 1);
	DrawTextEx(fontCN, title, { centerx - titleWidth.x / 2,y }, 70, 1, WHITE);
	y += 120;

	//副标题
	const char* subtitle = "- Bromato -";
	auto subWidth = MeasureTextEx(fontCN, subtitle, 40, 1);
	DrawTextEx(fontCN, subtitle, { centerx - subWidth.x / 2, y }, 40, 1, GRAY);
	y += 100;

	//制作人员列表
	for (auto& c : credits)
	{
		DrawTextEx(fontCN, c.name.c_str(), { 500, y }, 40, 1, WHITE);//c_str转义成c的字符串
		DrawTextEx(fontCN, c.role.c_str(), { 900, y }, 40, 1, GRAY);
		y += 80; // 每行间隔 80 像素
	}
	y += 40;

	//分割线
	DrawLine(300, y, 1620, y, GRAY);
	y += 60;

	//游戏信息标题
	const char* infoTitle = "游戏信息";
	auto infoWidth = MeasureTextEx(fontCN, infoTitle, 50, 1);
	DrawTextEx(fontCN, infoTitle, { centerx - infoWidth.x / 2, y }, 50, 1, WHITE);
	y += 90;

	// 游戏详细信息
	DrawTextEx(fontCN, ("版本号: " + version).c_str(), { 400, y }, 35, 1, WHITE);
	y += 55;
	DrawTextEx(fontCN, "开发引擎: raylib + C++", { 400, y }, 35, 1, WHITE);
	y += 55;
	DrawTextEx(fontCN, TextFormat("分辨率： %d x %d", GetScreenWidth(), GetScreenHeight()), { 400, y }, 35, 1, WHITE);
	y += 55;
	DrawTextEx(fontCN, TextFormat("帧率： %d", data.fps), { 400, y }, 35, 1, WHITE);
	y += 80;

	backButton.Draw();
}
