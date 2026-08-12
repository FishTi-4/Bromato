#pragma once

#include "Entity.h"

enum class EnemyName
{
	Normal1,
	Normal2,
	Ranged,
	Charge,
	Boss,
	Count,
};

class Enemy : public Entity
{
private:
	EnemyName name;
	float hp;
	float damage;
	float bulletDamage;
	float bulletSpeed;
	float bulletRadius;
	float shootInterval;
	float shootTimer;

public:
	Enemy();
	Enemy(EnemyName, float, float, float, float, float, float, float, float, TextureName);

	void Reset();
	void TakeDamage(float damage);
	void UpdateShootTimer();
	void ResetShootTimer();

	void SetName(EnemyName newName);
	void SetHP(float newHP);
	void SetDamage(float newDamage);
	void SetBulletDamage(float newDamage);
	void SetBulletSpeed(float newSpeed);
	void SetBulletRadius(float newRadius);
	void SetShootInterval(float newInterval);

	EnemyName GetName() const;
	float GetHP() const;
	float GetDamage() const;
	float GetBulletDamage() const;
	float GetBulletSpeed() const;
	float GetBulletRadius() const;
	float GetShootInterval() const;
	bool CanShoot() const;
};
