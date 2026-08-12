#include "SceneHistory.h"
#include <fstream>
#include <filesystem>
#include <cmath>
#include <map>
namespace fs = std::filesystem;

SceneHistory::SceneHistory() {
    RefreshSaveList();
    if (data.isDead == 1)
    {
        data.isDead = 0;
        RuntimeData saveData = data;
        saveData.dt = 0.0f;
        saveData.mouse = { 0,0 };

        int newId = 1;
        std::string newfile;
        do
        {
            newfile = std::string("save/save_") + TextFormat("%03d", newId) + ".dat";
            newId++;
        } while (fs::exists(newfile));

        std::ofstream file(newfile, std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(&saveData), sizeof(RuntimeData));
            RefreshSaveList();
            saveIndex = 0;
        }
    }
}

void SceneHistory::Input()
{
    // 按ESC退出
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetNextScene(SceneType::Menu);
        return;
    }

    // 上下方向键切换选中
    if (IsKeyPressed(KEY_DOWN))
    {
        if (!saveFileList.empty())
        {
            saveIndex++;
            if (saveIndex >= (int)saveFileList.size())
                saveIndex = (int)saveFileList.size() - 1;
        }
    }
    if (IsKeyPressed(KEY_UP))
    {
        if (!saveFileList.empty())
        {
            saveIndex--;
            if (saveIndex < 0)
                saveIndex = 0;
        }
    }

    // 按S键保存data (保留，可用于测试)

    // 返回按键
    /*if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouseRaw = GetMousePosition();
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        float scale = fmin((float)screenW / LOGICAL_WIDTH, (float)screenH / LOGICAL_HEIGHT);
        float renderW = LOGICAL_WIDTH * scale;
        float renderH = LOGICAL_HEIGHT * scale;

        float offsetX = (screenW - renderW) * 0.5f;
        float offsetY = (screenH - renderH) * 0.5f;

        Vector2 logicMouse;
        logicMouse.x = (mouseRaw.x - offsetX) / scale;
        logicMouse.y = (mouseRaw.y - offsetY) / scale;


        Rectangle backBtn = { 1450, 950, 300, 60 };
        if (CheckCollisionPointRec(logicMouse, backBtn))
        {
            SetNextScene(SceneType::Menu);
        }
    }*/
}


void SceneHistory::Update() {
    if (btn.Update())
    {
        SetNextScene(SceneType::Menu);
        audio.PlaySFX(SFXType::Push);
    }
}

static std::string GetWeaponDisplayName(WeaponName name)
{
    switch (name)
    {
    case WeaponName::Pistol:    return "手枪";
    case WeaponName::Smg:       return "冲锋枪";
    case WeaponName::Shotgun:   return "霰弹枪";
    case WeaponName::Scout:     return "狙击枪";
    default: return "未知武器";
    }
}

static std::string GetItemDisplayName(ItemName name)
{
    switch (name)
    {
    case ItemName::Healthmul: return "生命倍率";
    case ItemName::Defence:   return "防御";
    case ItemName::Damage:    return "伤害";
    case ItemName::Haste:     return "急速";
    case ItemName::Speed:     return "移速";
    case ItemName::Healthadd: return "生命加成";
    case ItemName::Glasses:   return "眼镜";
    case ItemName::Potato:    return "土豆";
    default: return "未知道具";
    }
}

static std::string GetCharacterDisplayName(Character name)
{
    switch (name)
    {
    case Character::Ox:         return "公牛";
    case Character::Doctor:     return "医生";
    case Character::Fighter:    return "斗士";
    case Character::Multiskill: return "多面手";
    case Character::Allskill:   return "全能";
    default: return "未知角色";
    }
}

static std::string GetMapDisplayName(Map name)
{
    switch (name)
    {
    case Map::Mp1: return "普通";
    case Map::Mp2: return "地牢";
    case Map::Mp3: return "沙漠";
    case Map::Mp4: return "泥潭";
    default: return "未知地图";
    }
}

void SceneHistory::Draw()
{
    ClearBackground(Color{ 16, 20, 28, 255 });

    DrawTextEx(fontCN, "历史记录", { 100,50 }, 50, 10, WHITE);

    DrawTextEx(fontCN, "记录列表", { 100, 120 }, 40, 10, YELLOW);

    int startY = 170;
    if (saveFileList.empty())
    {
        DrawTextEx(fontCN, "暂无历史记录", { 110, (float)startY }, 22, 1, GRAY);
    }

    for (int i = 0; i < (int)saveFileList.size(); i++)
    {
        if (i == saveIndex)
        {
            DrawRectangle(100, startY + i * 40, 400, 35, BLUE);
        }
        //std::string displayName = fs::path(saveFileList[i]).filename().string();
		std::string displayName = std::string("最近的第") + std::to_string(i + 1) + std::string("次游戏");
        DrawTextEx(fontCN, displayName.c_str(), { 110,(float)(startY + i * 40 + 8) }, 20, 1, WHITE);
    }

    DrawTextEx(fontCN, "按 ↑/↓ 键选择", { 100, 750 }, 40, 1, LIGHTGRAY);
    DrawTextEx(fontCN, "按 ESC 返回主菜单", { 100, 800 }, 40, 1, LIGHTGRAY);

    // ----------------右侧存档详情面板-------------------
    int panelX = 620;
    int panelY = 120;
    DrawTextEx(fontCN, "选中记录详情", { (float)panelX, (float)(panelY - 40) }, 30, 1, YELLOW);

    if (saveIndex >= 0 && saveIndex != previewIndex)
    {
        std::string path = saveFileList[saveIndex];
        std::ifstream file(path, std::ios::binary);
        if (file.is_open())
        {
            file.read(reinterpret_cast<char*>(&previewdata), sizeof(RuntimeData));
            previewIndex = saveIndex;
        }
    }

    if (saveIndex >= 0)
    {
        int lineY = panelY;

        DrawTextEx(fontCN, TextFormat("波次：%d", previewdata.wave), { (float)panelX, (float)lineY }, 25, 1, WHITE);
        //DrawTextEx(fontCN, TextFormat("波次：%d", previewdata.wave), { (float)panelX,(float)lineY }, 25, 1, WHITE);
        lineY += 35;
        DrawTextEx(fontCN, TextFormat("击杀：%d", previewdata.enemyKilled), { (float)panelX, (float)lineY }, 25, 1, WHITE);
        lineY += 35;
        DrawTextEx(fontCN, TextFormat("金币：%d", previewdata.coin), { (float)panelX, (float)lineY }, 25, 1, WHITE);
        lineY += 35;
        DrawTextEx(fontCN, TextFormat("角色：%s", GetCharacterDisplayName(previewdata.character).c_str()), { (float)panelX, (float)lineY }, 25, 1, WHITE);
        lineY += 35;
        DrawTextEx(fontCN, TextFormat("地图：%s", GetMapDisplayName(previewdata.map).c_str()), { (float)panelX, (float)lineY }, 25, 1, WHITE);
        lineY += 35;
        //DrawTextEx(fontCN, TextFormat("难度：%d", previewdata.difficulty), { (float)panelX, (float)lineY }, 25, 1, WHITE);
        lineY += 45;

        DrawTextEx(fontCN, "武器：", { (float)panelX, (float)lineY }, 25, 1, YELLOW);
        lineY += 35;
        for (int i = 0; i < Idx(WeaponName::Count); i++)
        {
            WeaponName weapon = (WeaponName)i;
            DrawTextEx(fontCN, TextFormat("%s 等级%d", GetWeaponDisplayName(weapon).c_str(), previewdata.weaponLevel[i]), { (float)panelX, (float)lineY }, 22, 1, WHITE);
            lineY += 30;
        }

        lineY += 15;
        DrawTextEx(fontCN, "道具：", { (float)panelX, (float)lineY }, 25, 1, YELLOW);
        lineY += 35;
        for (int i = 0; i < Idx(ItemName::Count); i++)
        {
            ItemName item = (ItemName)i;
            int x = panelX + i % 2 * 300;
            int y = lineY + i / 2 * 30;
            DrawTextEx(fontCN, TextFormat("%s x%d", GetItemDisplayName(item).c_str(), previewdata.itemCount[i]), { (float)x, (float)y }, 22, 1, WHITE);
        }
    }
    else
    {
        DrawTextEx(fontCN, "请从左侧选择记录", { (float)panelX, (float)panelY }, 25, 1, GRAY);
    }

    //----------------返回按钮-----------------
    /*Rectangle backBtn = { 1450, 950, 300, 60 };
    DrawRectangleRec(backBtn, DARKBLUE);
    DrawRectangleLinesEx(backBtn, 2, WHITE);
    const char* btnText = "返回主菜单";
    int textW = MeasureTextEx(fontCN, btnText, 30, 1).x;
    DrawTextEx(fontCN, btnText, { backBtn.x + (backBtn.width - textW) / 2.0f, backBtn.y + 15.0f }, 30, 1, WHITE);
    */
    btn.Draw();
}

void SceneHistory::RefreshSaveList()
{
    saveFileList.clear();
    saveIndex = -1;
    previewIndex = -1;

    if (!fs::exists("save"))
    {
        fs::create_directories("save");
        return;
    }


    std::map<std::string, fs::file_time_type> filemap;
    for (auto list : fs::directory_iterator("save")) {
        if (list.path().extension() == ".dat") {
            auto time = fs::last_write_time(list.path());
            std::string path = list.path().string();
            filemap[path] = time;
        }
    }

    while (filemap.size() > maxSize) {
        auto oldfile = filemap.begin();
        for (auto item = filemap.begin(); item != filemap.end(); ++item) {
            if (oldfile->second > item->second) {
                oldfile = item;
            }
        }
        fs::remove(oldfile->first);
        filemap.erase(oldfile);
    }

    while (!filemap.empty())
    {
        auto newfile = filemap.begin();
        for (auto item = filemap.begin(); item != filemap.end(); ++item)
        {
            if (newfile->second < item->second)
            {
                newfile = item;
            }
        }
        saveFileList.push_back(newfile->first);
        filemap.erase(newfile);
    }
}
