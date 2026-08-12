#pragma once

#include "Entity.h"

class Player : public Entity
{
private:
	float hp;
	float maxHP;
	float invincibleTimer;

public:
	Player();

	void Reset();
	void UpdateInvincibleTimer();
	void TakeDamage(float damage);

	void SetHP(float newHP);
	void SetMaxHP(float newMaxHP);

	float GetHP() const;
	float GetMaxHP() const;
	float GetInvincibleTimer() const;
	bool IsInvincible() const;
};
