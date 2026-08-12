#include "EntityPool.h"
#include "AssetsManager.h"
#include "EnemyManager.h"

extern EnemyManager enemies;
extern AssetsManager assets;

void DrawPoolEntity(const Entity& entity)
{
	float tm = (float)GetTime();
	const Texture2D& texture = assets.GetTexture(entity.GetTextureName());
	float radius = entity.GetRadius();


	float d1 = (abs(sin(tm * 5))) * 10;
	float d2 = (abs(cos(tm * 5))) * 10;


	Rectangle source = { 0,0,entity.GetLooking() ? (float)texture.width : -(float)texture.width,(float)texture.height };
	Rectangle destination = { entity.GetPosition().x - radius ,entity.GetPosition().y - radius,radius + radius + d1,radius + radius + d2 };
	DrawTexturePro(texture, source, destination, { 0,0 }, sin(tm * entity.GetSpeed() / 40) * 3, WHITE);
}

void DrawPoolEntity2(const Entity& entity)
{
	float tm = (float)GetTime();
	const Texture2D& texture = assets.GetTexture(entity.GetTextureName());
	float radius = entity.GetRadius();
	float d1 = 0;
	float d2 = 0;
	Rectangle source = { 0,0,entity.GetLooking() ? (float)texture.width : -(float)texture.width,(float)texture.height };
	Rectangle destination = { entity.GetPosition().x - radius + d1 ,entity.GetPosition().y - radius,radius + radius,radius + radius + d2 };
	DrawTexturePro(texture, source, destination, { 0,0 }, 0, WHITE);
}

EnemyPool::EnemyPool(int capacity) { enemies.resize(capacity); }

Enemy* EnemyPool::Create()
{
	for (Enemy& enemy : enemies)
	{
		if (!enemy.IsActive())
		{
			enemy.Reset();
			enemy.SetActive(1);
			return &enemy;
		}
	}
	return NULL;
}

void EnemyPool::Delete(Enemy* enemy) { if (enemy != NULL) enemy->Delete(); }
void EnemyPool::Clear() { for (Enemy& enemy : enemies) enemy.Reset(); }
void EnemyPool::UpdatePosition() { for (Enemy& enemy : enemies) if (enemy.IsActive()) enemy.UpdatePosition(); }

void EnemyPool::Draw() {
	for (Enemy& enemy : enemies)
	{
		if (enemy.IsActive()) {
			DrawPoolEntity(enemy);

			//----------------血条------------------
			Vector2 pos = enemy.GetPosition();
			float radius = enemy.GetRadius();

			const Enemy& enemyTemplate = ::enemies.GetEnemy(enemy.GetName());
			float maxHp = enemyTemplate.GetHP();
			float curHp = enemy.GetHP();

			float hpRatio = curHp / maxHp;
			if (hpRatio < 0) hpRatio = 0;

			float offsetY = radius + 6;

			Rectangle barBack = { pos.x - radius, pos.y + offsetY, radius * 2, 5 };
			Rectangle barFront = { barBack.x, barBack.y, radius * 2 * hpRatio, 5 };

			DrawRectangleRec(barBack, DARKGRAY);
			DrawRectangleRec(barFront, RED);
			//--------------------------------------

		}
	}
}


int EnemyPool::GetActiveCount() const { int count = 0; for (const Enemy& enemy : enemies) if (enemy.IsActive()) count++; return count; }
std::vector<Enemy>& EnemyPool::GetEntities() { return enemies; }

BulletPool::BulletPool(int capacity) { bullets.resize(capacity); }

Bullet* BulletPool::Create()
{
	for (Bullet& bullet : bullets)
	{
		if (!bullet.IsActive())
		{
			bullet.Reset();
			bullet.SetActive(1);
			return &bullet;
		}
	}
	return NULL;
}

void BulletPool::Delete(Bullet* bullet) { if (bullet != NULL) bullet->Delete(); }
void BulletPool::Clear() { for (Bullet& bullet : bullets) bullet.Reset(); }
void BulletPool::UpdatePosition() { for (Bullet& bullet : bullets) if (bullet.IsActive()) bullet.UpdatePosition(); }
void BulletPool::Draw() {
	for (Bullet& bullet : bullets) if (bullet.IsActive())

		DrawPoolEntity2(bullet);

}

int BulletPool::GetActiveCount() const { int count = 0; for (const Bullet& bullet : bullets) if (bullet.IsActive()) count++; return count; }
std::vector<Bullet>& BulletPool::GetEntities() { return bullets; }



CoinPool::CoinPool(int capacity) { coins.resize(capacity); }

Coin* CoinPool::Create()
{
	for (Coin& coin : coins)
	{
		if (!coin.IsActive())
		{
			coin.Reset();
			coin.SetActive(1);
			return &coin;
		}
	}
}

void CoinPool::Draw() { for (Coin& coin : coins)if (coin.IsActive())DrawPoolEntity(coin); }
std::vector<Coin>& CoinPool::GetEntities() { return coins; }
