#include "SceneSelection.h"

static const char* charNames[] = {
	"公牛",
	"医生",
	"斗士",
	"多面手",
	"全能"
};
static const char* mapNames[] = {
	"普通",
	"地牢",
	"沙漠",
	"泥潭"
};

static void DrawAttributePanel(const Attribute& a, float x, float y)
{
	int line = 0;
	if (a.hpMul != 0) DrawTextEx(fontCN, TextFormat("最大生命：%+.0f%%", a.hpMul * 100), { x,y + line++ * 35 }, 28, 1, WHITE);
	if (a.hpAdd != 0) DrawTextEx(fontCN, TextFormat("最大生命：%+.0f", a.hpAdd), { x,y + line++ * 35 }, 28, 1, WHITE);
	if (a.defenceMul != 0) DrawTextEx(fontCN, TextFormat("防御：%+.0f%%", a.defenceMul * 100), { x,y + line++ * 35 }, 28, 1, WHITE);
	if (a.damageMul != 0) DrawTextEx(fontCN, TextFormat("伤害：%+.0f%%", a.damageMul * 100), { x,y + line++ * 35 }, 28, 1, WHITE);
	if (a.hasteMul != 0) DrawTextEx(fontCN, TextFormat("攻速：%+.0f%%", a.hasteMul * 100), { x,y + line++ * 35 }, 28, 1, WHITE);
	if (a.speedMul != 0) DrawTextEx(fontCN, TextFormat("移速：%+.0f%%", a.speedMul * 100), { x,y + line++ * 35 }, 28, 1, WHITE);
}//玩家属性绘制

SceneSelection::SceneSelection()
	:startButton({ 1640,950 }, 0.4f, "开始"), backButton({ 100,950 }, 0.4f, "返回")
{
	startButton.Disable();
}

// 计算第 i 张角色卡片在屏幕上的位置
Rectangle SceneSelection::GetCharRect(int i) const
{
	float w = 240, h = 300, gap = 30;//每张卡宽度高度和水平间距
	float startX = (1920 - (5 * w + 4 * gap)) / 2;  // 让整行居中
	return { startX + i * (w + gap), 120, w, h };//左上角xy宽高
}

// 计算第 i 张地图卡片在屏幕上的位置（4 张卡片水平均匀排列）
Rectangle SceneSelection::GetMapRect(int i) const
{
	float w = 260, h = 200, gap = 40;
	float startX = (1920 - (4 * w + 3 * gap)) / 2;
	return { startX + i * (w + gap), 500, w, h };
}



void SceneSelection::Input()
{
	for (int i = 0;i < Idx(Character::Count);i++) {
		if (CheckCollisionPointRec(data.mouse, GetCharRect(i)) &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			selectedChar = i;
			data.character = static_cast<Character>(i);
			audio.PlaySFX(SFXType::Select);
		}
	}
	// 检测鼠标是否点击了某张地图卡片
	for (int i = 0; i < Idx(Map::Count); i++)
	{
		if (CheckCollisionPointRec(data.mouse, GetMapRect(i)) &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			selectedMap = i;
			data.map = static_cast<Map>(i);
			audio.PlaySFX(SFXType::Select);
		}
	}

	// 角色和地图都选好了才让点"开始"
	if (selectedChar >= 0 && selectedMap >= 0)
		startButton.Enable();

	// 按钮点击：开始 -> 进游戏，返回 -> 回菜单
	if (startButton.Update())
		SetNextScene(SceneType::Game), audio.PlaySFX(SFXType::Push);
	if (backButton.Update())
		SetNextScene(SceneType::Menu), audio.PlaySFX(SFXType::Push);
}

void SceneSelection::Draw()
{
	ClearBackground(Color{ 16, 20, 28, 255 });

	// 顶部标题
	const char* title = "选择地图和角色";
	int aa = MeasureTextEx(fontCN, title, 50, 1).x;
	DrawTextEx(fontCN, title, { (float)(1920 - aa) / 2,40 }, 50, 1, WHITE);

	// 5张角色卡片
	for (int i = 0; i < Idx(Character::Count); i++)
	{
		Rectangle r = GetCharRect(i);

		// 选中和未选中的卡片不同
		Color cardColor = (i == selectedChar) ? DARKBLUE : DARKGRAY;
		DrawRectangleRec(r, cardColor);

		//资源管理器赛高
		// 拿角色贴图，放到卡片内部
		const Attribute& a = characters.GetCharacter(static_cast<Character>(i));
		Texture2D tex = assets.GetTexture(a.textureName);
		//神秘缩放
		float maxTexH = r.height - 55;//限高，不然超框了
		float scale = fminf((r.width - 20) / tex.width, maxTexH / tex.height);
		float texH = tex.height * scale;

		DrawTexturePro(tex,
			{ 0, 0, (float)tex.width, (float)tex.height },//取图
			{ r.x + 10, r.y + 10, r.width - 20, texH },//卡片留10边距
			{ 0, 0 }, 0, WHITE);

		// 卡片下方居中显示名字
		Vector2 nameSize = MeasureTextEx(fontCN, charNames[i], 25, 1);
		DrawTextEx(fontCN, charNames[i], { r.x + (r.width - nameSize.x) / 2,r.y + r.height - 45 }, 25, 1, WHITE);

		// 选中卡片用金粗边，未选中用灰细bian
		if (i == selectedChar)
			DrawRectangleLinesEx(r, 4, GOLD);
		else
			DrawRectangleLinesEx(r, 1, GRAY);
	}

	//绘制4张地图卡片 
	for (int i = 0; i < Idx(Map::Count); i++)
	{
		Rectangle r = GetMapRect(i);
		Color cardColor = (i == selectedMap) ? DARKGREEN : DARKGRAY;
		DrawRectangleRec(r, cardColor);

		const Attribute& attr = maps.GetMap(static_cast<Map>(i));
		Texture2D tex = assets.GetTexture(attr.textureName);
		float maxTexH = r.height - 55;//限高2.0
		float scale = fminf((r.width - 20) / tex.width, maxTexH / tex.height);
		float texH = tex.height * scale;
		DrawTexturePro(tex,
			{ 0, 0, (float)tex.width, (float)tex.height },
			{ r.x + 10, r.y + 10, r.width - 20, texH },
			{ 0, 0 }, 0, WHITE);

		Vector2 nameSize = MeasureTextEx(fontCN, mapNames[i], 25, 1);
		DrawTextEx(fontCN, mapNames[i], { r.x + (r.width - nameSize.x) / 2,r.y + r.height - 45 }, 25, 1, WHITE);

		// 选中卡片用金粗边，未选中用灰细边
		if (i == selectedMap)
			DrawRectangleLinesEx(r, 4, GOLD);
		else
			DrawRectangleLinesEx(r, 1, GRAY);
	}

	// ---- 显示选中角色和地图的属性 ----
	float y = 730.0;
	if (selectedChar >= 0)
		DrawAttributePanel(characters.GetCharacter(static_cast<Character>(selectedChar)), 300.0, y);
	if (selectedMap >= 0)
		DrawAttributePanel(maps.GetMap(static_cast<Map>(selectedMap)), 900.0, y);

	startButton.Draw();
	backButton.Draw();
}
