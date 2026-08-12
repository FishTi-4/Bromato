#include "context.h"

#include "Scene.h"
#include "SceneMenu.h"
#include "SceneGame.h"
#include "SceneSummary.h"
#include "SceneShop.h"
#include "SceneSelection.h"
#include "SceneHistory.h"
#include "SceneAbout.h"
#include "Settings.h"

#include <algorithm>

#include<iostream>
#include<fstream>


Rectangle GetDisplayRegion();
Vector2 GetLogicalMouse();
void UpdateSceneType(std::unique_ptr<Scene>& scene);
void UpdateGame(std::unique_ptr<Scene>& scene);
void DrawLogicScreen(std::unique_ptr<Scene>& scene, const RenderTexture2D& logicalScreen);
void DrawRealisticScreen(const RenderTexture2D& logicalScreen);
void ReadSettings();
void WriteSettings();


int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "Bromato");
	SetWindowMinSize(640, 360);
	SetTargetFPS(60);
	RenderTexture2D logicalScreen = LoadRenderTexture(LOGICAL_WIDTH, LOGICAL_HEIGHT); // 双线性过滤 缩放后更加平滑 但可能有点模糊
	SetExitKey(KEY_NULL);
	SetTextureFilter(logicalScreen.texture, TEXTURE_FILTER_BILINEAR);
	assets.Load();
	std::unique_ptr<Scene> scene = std::make_unique<SceneMenu>();
	ReadSettings();
	audio.Init();
	chineseFontHandler.Load();
	SetTargetFPS(data.fps);
	while (!WindowShouldClose() && scene->GetNextScene() != SceneType::Exit)
	{
		if (IsKeyPressed(KEY_F11)) ToggleBorderlessWindowed();
		UpdateGame(scene);
		audio.Update();
		DrawLogicScreen(scene,logicalScreen);
		DrawRealisticScreen(logicalScreen);
	}
	chineseFontHandler.Unload();
	WriteSettings();
	audio.Shutdown();
	assets.Unload();
	UnloadRenderTexture(logicalScreen);
	CloseWindow();
	return 0;
}

void UpdateSceneType(std::unique_ptr<Scene>& scene)
{
	if (scene->GetNextScene() == SceneType::None) return;
	switch (scene->GetNextScene())
	{
	case SceneType::None:
		return;
	case SceneType::Menu:
		scene = std::make_unique<SceneMenu>();
		return;
	case SceneType::Game:
		scene = std::make_unique<SceneGame>();
		return;
	case SceneType::Summary:
		scene = std::make_unique<SceneSummary>();
		return;
	case SceneType::Shop:
		scene = std::make_unique<SceneShop>();
		return;
	case SceneType::Selection:
		scene = std::make_unique<SceneSelection>();
		return;
	case SceneType::Settings:
		scene = std::make_unique<SceneSettings>();
		return;
	case SceneType::History:
		scene = std::make_unique<SceneHistory>();
		return;
	case SceneType::Exit:
		return;
	case SceneType::About:
		scene = std::make_unique<SceneAbout>();
		return;
	}
}


void UpdateGame(std::unique_ptr<Scene>& scene)
{
	UpdateSceneType(scene);
	data.dt = GetFrameTime();
	data.mouse = GetLogicalMouse();
	scene->Input();
	scene->Update();
}

void DrawLogicScreen(std::unique_ptr<Scene>& scene, const RenderTexture2D& logicalScreen)
{
	BeginTextureMode(logicalScreen);
	ClearBackground(BLACK);
	scene->Draw();
	EndTextureMode();
}

void DrawRealisticScreen(const RenderTexture2D& logicalScreen)
{
	BeginDrawing();
	ClearBackground(BLACK);
	Rectangle logicalToBeDrawArea = { 0,0,LOGICAL_WIDTH, -LOGICAL_HEIGHT };
	DrawTexturePro(logicalScreen.texture, logicalToBeDrawArea, GetDisplayRegion(), { 0,0 }, 0, WHITE);
	EndDrawing();
}

Rectangle GetDisplayRegion()
{
	float realisticWidth = GetScreenWidth();
	float realisticHeight = GetScreenHeight();
	float scaleX = 1.0 * realisticWidth / LOGICAL_WIDTH;
	float scaleY = 1.0 * realisticHeight / LOGICAL_HEIGHT;
	float scale = std::min(scaleX, scaleY);
	float displayWidth = LOGICAL_WIDTH * scale;
	float displayHeight = LOGICAL_HEIGHT * scale;
	float offsetX = (realisticWidth - displayWidth) / 2;
	float offsetY = (realisticHeight - displayHeight) / 2;
	return { offsetX,offsetY,displayWidth,displayHeight };
}

Vector2 GetLogicalMouse()
{
	const Vector2 mouse = GetMousePosition();
	const Rectangle displayRegion = GetDisplayRegion();

	return {
		(mouse.x - displayRegion.x) * LOGICAL_WIDTH / displayRegion.width,
		(mouse.y - displayRegion.y) * LOGICAL_HEIGHT / displayRegion.height
	};
}

void ReadSettings()
{
	std::ifstream file("config/settings.txt");
	//settings没打开或者发生读写错误，设为默认值
	if (!file)
	{
		data.fps = 60, data.volume = 50;
		return;
	}

	file.seekg(0, std::ios::end);		//把读取指针移到文本末尾
	//settings中没有内容，设为默认值
	if (file.tellg() == 0)
	{
		data.fps = 60, data.volume = 50;
		return;
	}
	file.seekg(0, std::ios::beg);

	file >> data.fps >> data.volume;
	if (data.fps < 0 || data.fps > 100) data.fps = 60;
	if (data.volume < 0 || data.volume > 100) data.volume = 70;
	file.close();
}


void WriteSettings()
{
	std::ofstream file("config/settings.txt");
	if (file.is_open())
		file << data.fps << " " << data.volume;
	file.close();
	return;
}