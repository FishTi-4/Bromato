#include "SceneSummary.h"

static const char* GetSummaryCharacterName(Character character)
{
	switch (character)
	{
	case Character::Ox: return "公牛";
	case Character::Doctor: return "医生";
	case Character::Fighter: return "斗士";
	case Character::Multiskill: return "多面手";
	case Character::Allskill: return "全能";
	}
}

static const char* GetSummaryMapName(Map map)
{
	switch (map)
	{
	case Map::Mp1: return "默认";
	case Map::Mp2: return "地牢";
	case Map::Mp3: return "沙漠";
	case Map::Mp4: return "泥潭";
	}
}

static void DrawSummaryText(const char* text, float x, float y, float fontSize, Color color)
{
	DrawTextEx(fontCN,text,{ x,y },fontSize,1,color);
}

void SceneSummary::Input()
{
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
		SetNextScene(SceneType::History);
}

void SceneSummary::Update()
{
	if (backToMenu.Update()) SetNextScene(SceneType::History), audio.PlaySFX(SFXType::Push);
}

void SceneSummary::Draw()
{
	const Texture2D& background =
		assets.GetTexture(maps.GetMap(data.map).textureName);

	Rectangle source = { 0,0,(float)background.width,(float)background.height };
	Rectangle destination = { 0,0,LOGICAL_WIDTH,LOGICAL_HEIGHT };
	DrawTexturePro(background,source,destination,{ 0,0 },0,WHITE);

	DrawRectangle(0,0,LOGICAL_WIDTH,LOGICAL_HEIGHT,Color{ 0,0,0,180 });

	Rectangle shadow = { 320,170,1320,650 };
	DrawRectangleRounded(shadow,0.08,16,Color{ 0,0,0,130 });

	Rectangle panel = { 300,150,1320,650 };
	DrawRectangleRounded(panel,0.08,16,Color{ 32,36,48,245 });
	DrawRectangleRoundedLinesEx(panel,0.08,16,4,Color{ 214,158,62,255 });

	DrawCenteredText("游戏结束",45,76,Color{ 225,75,75,255 });

	DrawSummaryText("本局数据",390,200,36,Color{ 245,190,75,255 });
	DrawSummaryText("最终属性",1030,200,36,Color{ 245,190,75,255 });
	DrawRectangle(360,250,520,3,Color{ 214,158,62,180 });
	DrawRectangle(1000,250,520,3,Color{ 214,158,62,180 });
	DrawRectangle(958,190,3,550,Color{ 214,158,62,100 });

	DrawSummaryText("到达波次",390,285,30,LIGHTGRAY);
	DrawSummaryText(TextFormat("%d",data.wave),760,285,32,WHITE);
	DrawSummaryText("生存时间",390,345,30,LIGHTGRAY);
	DrawSummaryText(TextFormat("%.1f 秒",data.totalTimer),760,345,32,WHITE);
	DrawSummaryText("击败敌人",390,405,30,LIGHTGRAY);
	DrawSummaryText(TextFormat("%d",data.enemyKilled),760,405,32,WHITE);
	DrawSummaryText("持有金币",390,465,30,LIGHTGRAY);
	DrawSummaryText(TextFormat("%d",data.coin),760,465,32,GOLD);
	DrawSummaryText("角色",390,545,30,LIGHTGRAY);
	DrawSummaryText(GetSummaryCharacterName(data.character),650,545,32,WHITE);
	DrawSummaryText("地图",390,605,30,LIGHTGRAY);
	DrawSummaryText(GetSummaryMapName(data.map),650,605,32,WHITE);

	float maxHP = data.attribute.hpAdd * data.attribute.hpMul;
	DrawSummaryText("最大生命",1030,280,27,LIGHTGRAY);
	DrawSummaryText(TextFormat("%.0f",maxHP),1410,280,28,WHITE);
	DrawSummaryText("防御",1030,325,27,LIGHTGRAY);
	DrawSummaryText(TextFormat("x%.2f",data.attribute.defenceMul),1410,325,28,WHITE);
	DrawSummaryText("伤害",1030,370,27,LIGHTGRAY);
	DrawSummaryText(TextFormat("x%.2f",data.attribute.damageMul),1410,370,28,WHITE);
	DrawSummaryText("攻速",1030,415,27,LIGHTGRAY);
	DrawSummaryText(TextFormat("x%.2f",data.attribute.hasteMul),1410,415,28,WHITE);
	DrawSummaryText("移速",1030,460,27,LIGHTGRAY);
	DrawSummaryText(TextFormat("x%.2f",data.attribute.speedMul),1410,460,28,WHITE);

	DrawSummaryText("武器等级",1030,540,28,Color{ 245,190,75,255 });
	DrawSummaryText(TextFormat("手枪 %d",data.weaponLevel[Idx(WeaponName::Pistol)]),1030,585,24,WHITE);
	DrawSummaryText(TextFormat("冲锋枪 %d",data.weaponLevel[Idx(WeaponName::Smg)]),1300,585,24,WHITE);
	DrawSummaryText(TextFormat("霰弹枪 %d",data.weaponLevel[Idx(WeaponName::Shotgun)]),1030,625,24,WHITE);
	DrawSummaryText(TextFormat("狙击枪 %d",data.weaponLevel[Idx(WeaponName::Scout)]),1300,625,24,WHITE);

	int itemCount = 0;
	for (int i = 0; i < Idx(ItemName::Count); i++) itemCount += data.itemCount[i];
	DrawSummaryText(TextFormat("持有道具 %d",itemCount),1030,685,24,LIGHTGRAY);

	backToMenu.Draw();
	const char* buttonText = "查看历史";
	Vector2 buttonTextSize = MeasureTextEx(fontCN,buttonText,40,1);
	DrawSummaryText(buttonText,735 + (450 - buttonTextSize.x) / 2,850 + (140 - buttonTextSize.y) / 2,40,BLACK);
	DrawCenteredText("按回车或退出键查看历史",1025,24,GRAY);
}

void SceneSummary::DrawCenteredText(
	const char* text,
	int y,
	int fontSize,
	Color color)
{
	Vector2 textSize = MeasureTextEx(fontCN,text,fontSize,1);
	DrawTextEx(fontCN,text,{ LOGICAL_WIDTH / 2 - textSize.x / 2,(float)y },fontSize,1,color);
}
