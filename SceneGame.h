#pragma once

#include "context.h"
#include <cmath>
#include <deque>
#include <string>
#include <vector>

const float MAP_WIDTH = LOGICAL_WIDTH * 1.5;
const float MAP_HEIGHT = LOGICAL_HEIGHT * 1.5;
const float PLAYER_RADIUS = 35;
const float ENEMY_SPAWN_INTERVAL = 1;
const float ROUND_TIME = 40;

class SceneGame : public Scene
{
private:
public:
	Camera2D camera;

	class DamageText
	{
	public:
		void Draw();
		void Add(int damage, Vector2 position);

	private:
		struct Text
		{
			Font font;
			std::string text;
			Vector2 position;
			Vector2 origin;
			float rotation;
			float size;
		};

		std::deque<double> deleteTimes;
		std::deque<Text> texts;
	};

	struct BulletSpawnRequest
	{
		Vector2 position;
		Vector2 direction;
		float speed;
		float radius;
		float damage;
		TextureName texture;
	};

	struct DamageRequest
	{
		Player* playerTarget;
		Enemy* enemyTarget;
		float damage;
	};

	struct EnemySpawnRequest
	{
		EnemyName name;
		Vector2 position;
	};

	bool isPaused = 0;
	Button pauseButton{ { LOGICAL_WIDTH - 210,30 },{ 180,70 },"暂停" };
	Button resumeButton{ { LOGICAL_WIDTH / 2 - 350,610 },{ 700,150 },"返回游戏" };
	Button endGameButton{ { LOGICAL_WIDTH / 2 - 350,800 },{ 700,150 },"结束游戏" };

	EnemyPool enemyPool{ 1000 };
	BulletPool enemyBulletPool{ 1000 };
	BulletPool playerBulletPool{ 1000 };
	Player player;
	CoinPool coinPool{ 2000 };


	std::vector<BulletSpawnRequest> enemyBulletSpawnRequests;
	std::vector<BulletSpawnRequest> playerBulletSpawnRequests;
	std::vector<DamageRequest> damageRequests;
	std::vector<EnemySpawnRequest> enemySpawnRequests;
	float enemySpawnTimer = 0;
	float weaponShootTimers[Idx(WeaponName::Count)] = {};
	DamageText damageTexts;


	Vector2 RotateVector(Vector2 v, float angle);

	//夜游
	SceneGame();
	void Input() override;
	void Initialize();
	void Update() override;
	void CheckEnd();
	void Draw() override;

	//雪梨
	void PlayerUpdate();

	//咸鱼
	Enemy* FindEnemy(float);
	float CalculateWeaponDamage(WeaponName weaponName);
	void TryShot(WeaponName weaponName);
	void ProcessAllWeapon();

	// 呆猫
	void EnemyUpdate();

	// 黑蛋
	void HandleBulletSpawnRequests(BulletPool& pool, std::vector<BulletSpawnRequest>& requests);
	void BulletUpdate();

	// 蒟蒻
	void CollisionUpdate();
};
