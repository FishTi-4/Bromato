#pragma once

#include "Enemy.h"
#include "Bullet.h"
#include "Coin.h"
#include <vector>

class EnemyPool
{
private:
	std::vector<Enemy> enemies;

public:
	EnemyPool(int capacity);
	Enemy* Create();
	void Delete(Enemy* enemy);
	void Clear();
	void UpdatePosition();
	void Draw();
	int GetActiveCount() const;
	std::vector<Enemy>& GetEntities();
};

class BulletPool
{
private:
	std::vector<Bullet> bullets;

public:
	BulletPool(int capacity);
	Bullet* Create();
	void Delete(Bullet* bullet);
	void Clear();
	void UpdatePosition();
	void Draw();
	int GetActiveCount() const;
	std::vector<Bullet>& GetEntities();
};


class CoinPool
{
private:
	std::vector<Coin> coins;

public:
	CoinPool(int capacity);
	Coin* Create();
	void Draw();
	int GetActiveCount() const;
	std::vector<Coin>& GetEntities();
};