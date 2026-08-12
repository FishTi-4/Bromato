#include "Enemy.h"
#include "RuntimeData.h"

extern RuntimeData data;

Enemy::Enemy() { Reset(); }

Enemy::Enemy(EnemyName nn, float spd, float rds, float nhp, float dmg, float bdmg, float bspd, float brds, float sitv, TextureName nt)
{
	Reset();
	SetSpeed(spd);
	SetRadius(rds);
	SetTextureName(nt);
	name = nn;
	hp = nhp;
	damage = dmg;
	bulletDamage = bdmg;
	bulletSpeed = bspd;
	bulletRadius = brds;
	shootInterval = sitv;
	shootTimer = 0;
}

void Enemy::Reset()
{
	Entity::Reset();
	name = EnemyName::Normal1;
	hp = 0;
	damage = 0;
	bulletDamage = 0;
	bulletSpeed = 0;
	bulletRadius = 0;
	shootInterval = 0;
	shootTimer = 0;
}

void Enemy::TakeDamage(float damage)
{
	if (!IsActive() || damage <= 0) return;
	hp -= damage;
	if (hp < 0) hp = 0;
}

void Enemy::UpdateShootTimer() { if (shootTimer > 0) shootTimer -= data.dt; }

void Enemy::ResetShootTimer() { shootTimer = shootInterval; }

void Enemy::SetName(EnemyName newName) { name = newName; }
void Enemy::SetHP(float newHP) { hp = newHP; }
void Enemy::SetDamage(float newDamage) { damage = newDamage; }
void Enemy::SetBulletDamage(float newDamage) { bulletDamage = newDamage; }
void Enemy::SetBulletSpeed(float newSpeed) { bulletSpeed = newSpeed; }
void Enemy::SetBulletRadius(float newRadius) { bulletRadius = newRadius; }
void Enemy::SetShootInterval(float newInterval) { shootInterval = newInterval; }

EnemyName Enemy::GetName() const { return name; }
float Enemy::GetHP() const { return hp; }
float Enemy::GetDamage() const { return damage; }
float Enemy::GetBulletDamage() const { return bulletDamage; }
float Enemy::GetBulletSpeed() const { return bulletSpeed; }
float Enemy::GetBulletRadius() const { return bulletRadius; }
float Enemy::GetShootInterval() const { return shootInterval; }
bool Enemy::CanShoot() const { return shootTimer <= 0 && shootInterval > 0; }
