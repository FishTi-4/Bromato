#include "Settings.h"

extern RuntimeData data;

SceneSettings::SceneSettings()
    :backButton("返回", 777, 925, [this]() { SetNextScene(SceneType::Menu), audio.PlaySFX(SFXType::Push); }),
      fullWindows("全屏", 777, 800, [this]() { ToggleFullscreen(); })
{
    button_optings.emplace_back(BarOptions(&data.volume, { 460, 370 }, 900, 60, "音量"));
    button_optings.emplace_back(BarOptions(&data.fps, { 460, 570 }, 900, 60, "帧率限制"));
}

void SceneSettings::Input()
{
}

void SceneSettings::Update()
{
    for (auto& x : button_optings) {
        x.update();
    }

    if (data.fps < 1) data.fps = 1;
    SetTargetFPS(data.fps);
    SetMasterVolume(data.volume / 100.0f);

    backButton.handle_input();
    fullWindows.handle_input();

}

void SceneSettings::Draw()
{
    ClearBackground(Color{ 16, 20, 28, 255 });

    Rectangle panel = { 280, 150, 1360, 630 };
    DrawRectangleRounded(panel, 0.04f, 12, Color{ 27, 33, 44, 255 });
    DrawRectangleRoundedLinesEx(panel, 0.04f, 12, 3, Color{ 80, 95, 115, 255 });

    const char* title = "设置";
    int titleWidth = MeasureTextEx(fontCN, title, 64, 1).x;
    DrawTextEx(fontCN, title, { (float)(LOGICAL_WIDTH - titleWidth) / 2,60 }, 64, 1, WHITE);
    DrawTextEx(fontCN, "拖动滑条修改数值", { 460,230 }, 30, 1, Color{ 170, 180, 195, 255 });
    
    

    for (auto& x : button_optings) {
        x.Draw();
    }
    backButton.draw();
    fullWindows.draw();
}
