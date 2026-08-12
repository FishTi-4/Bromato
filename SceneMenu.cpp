#include "SceneMenu.h"

	SceneMenu::SceneMenu()
		: btnStart({ 150, 260 }, "")
		, btnSettings({ 150, 430 }, "")
		, btnHistory({ 150, 600 }, "")
		, btnAbout({ 150 , 770 }, "")
		, btnExit({ 150, 940 }, "")
	{
		data.wave = 0;
		data.enemyKilled = 0;
		data.timer = 0;
		data.totalTimer = 0;
		data.coin = 0;
		data.isDead = 0;
		data.waveOver = 0;
		for (int i = 0; i < Idx(WeaponName::Count); i++) data.weaponLevel[i] = 0;
		for (int i = 0; i < Idx(ItemName::Count); i++) data.itemCount[i] = 0;
	}

void SceneMenu::Input()
{
	if (IsKeyPressed(KEY_ESCAPE)) SetNextScene(SceneType::Exit);
}

void SceneMenu::Draw()
{
	//大背景
	Texture2D bg2 = assets.GetTexture(TextureName::Bg2);
	float bg2Overscan = 1920 + 20;		// 左右各 10 像素摆动余量
	float bg2Height = bg2Overscan * bg2.height / bg2.width;		// 按比例算高度 = 1940 * 381/464 ≈ 1593
	float bg2Y = (1080 - bg2Height) / 2;	// 垂直居中
	float bg2Offset = 10 * sin(data.timer * 0.25);		// 慢速左右摆动 -10~10
	Rectangle bg2Dest = { bg2Offset - 10, bg2Y, bg2Overscan, bg2Height };
	Rectangle bg2Src = { 0, 0, (float)bg2.width, (float)bg2.height };
	DrawTexturePro(bg2, bg2Src, bg2Dest, { 0,0 }, 0, Color{ 255, 255, 255, 153 });  // 153 = 255×0.6 设置60%的透明度

	//中间背景
	Texture2D bg3 = assets.GetTexture(TextureName::Bg3);
	float bg3Overscan = 1920 + 40;		// 左右各 20 像素余量
	float bg3Height = bg3Overscan * bg3.height / bg3.width;		// 1960 * 940/2374 ≈ 776
	float bg3Y = (1080 - bg3Height) / 2;	// 垂直居中
	float bg3Offset = 20 * sin(data.timer * 0.5);	// 中速摆动 -20~20
	Rectangle bg3Dest = { bg3Offset - 20, bg3Y, bg3Overscan, bg3Height };
	Rectangle bg3Src = { 0, 0, (float)bg3.width, (float)bg3.height };
	DrawTexturePro(bg3, bg3Src, bg3Dest, { 0,0 }, 0, WHITE);

	//顶部logo
	Texture2D logo = assets.GetTexture(TextureName::Logo_new);
	Rectangle logoSrc = { 0, 0, (float)logo.width, (float)logo.height };
	Rectangle logoDest = { 0, -30, 1920, 405 };
	DrawTexturePro(logo, logoSrc, logoDest, { 0,0 }, 0, WHITE);

	//底部背景
	Texture2D bg1 = assets.GetTexture(TextureName::Bg1);
	float bg1Overscan = 1920 + 80;		// 左右各 40 像素余量
	float bg1Height = bg1Overscan * bg1.height / bg1.width;		// 2000 * 338/1841 ≈ 367
	float bg1Y = 1080 - bg1Height;		// 贴底部
	float bg1Offset = 40 * sin(data.timer * 1.0);		// 快速摆动 -40~40
	Rectangle bg1Dest = { bg1Offset - 40, bg1Y, bg1Overscan, bg1Height };
	Rectangle bg1Src = { 0, 0, (float)bg1.width, (float)bg1.height };
	DrawTexturePro(bg1, bg1Src, bg1Dest, { 0,0 }, 0, WHITE);

	// 界面charactor
	Texture2D ch = assets.GetTexture(TextureName::Charactor);
	float chScaleX = 1.0 + 0.02 * sin(data.timer * 2.4);		//不对称缩放（ X ）
	float chScaleY = 1.0 - 0.02 * sin(data.timer * 2.4);		//不对称缩放（ Y ）
	float chW = ch.width * chScaleX;
	float chH = ch.height * chScaleY;
	float chX = 1052;
	float chY = 648;
	Rectangle chDest = { chX, chY, chW, chH };
	Rectangle chSrc = { 0, 0, (float)ch.width, (float)ch.height };
	DrawTexturePro(ch, chSrc, chDest, { chW / 2, chH / 2 }, 0, WHITE);    // 以图片中心为锚点缩放

	//测量位置用的代码
/*	ClearBackground(BLACK);		// 临时：在左上角显示鼠标坐标
	DrawTextEx(fontCN, TextFormat("X: %.0f  Y: %.0f", data.mouse.x,data.mouse.y), { 10,10 }, 40, 1, GREEN);
	DrawRectangle(data.mouse.x, data.mouse.y, 450, 140, RED);*/

	//按钮
	btnStart.Draw();
	btnSettings.Draw();
	btnHistory.Draw();
	btnAbout.Draw();
	btnExit.Draw();

	DrawTextEx(fontCN, "开始游戏", { 290,310 }, 42, 1, BLACK);
	DrawTextEx(fontCN, "设置", { 330,480 }, 42, 1, BLACK);
	DrawTextEx(fontCN, "历史记录", { 290,650 }, 42, 1, BLACK);
	DrawTextEx(fontCN, "  关于", { 290,820 }, 42, 1, BLACK);
	DrawTextEx(fontCN, "退出游戏", { 290,990 }, 42, 1, BLACK);
}


void SceneMenu::Update()
{
	data.timer += data.dt;
	if (btnStart.Update())
		SetNextScene(SceneType::Selection), audio.PlaySFX(SFXType::Push);
	if (btnSettings.Update())
		SetNextScene(SceneType::Settings), audio.PlaySFX(SFXType::Push);
	if (btnHistory.Update())
		SetNextScene(SceneType::History), audio.PlaySFX(SFXType::Push);
	if (btnAbout.Update())
		SetNextScene(SceneType::About), audio.PlaySFX(SFXType::Push);
	if (btnExit.Update())
		SetNextScene(SceneType::Exit), audio.PlaySFX(SFXType::Push);
	// ----------------------------------------test ---------------------
	audio.PlayBGM(BGMTrack::Background);
}
