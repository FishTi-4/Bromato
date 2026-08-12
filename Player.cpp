#include "Player.h"
#include "RuntimeData.h"

extern RuntimeData data;

const float PLAYER_INVINCIBLE_TIME = 0.5;

Player::Player() { Reset(); }

void Player::Reset()
{
	Entity::Reset();
	hp = 0;
	maxHP = 0;
	invincibleTimer = 0;
}

void Player::UpdateInvincibleTimer()
{
	if (invincibleTimer <= 0) return;	
	invincibleTimer -= data.dt;
	if (invincibleTimer < 0) invincibleTimer = 0;
}

void Player::TakeDamage(float damage)
{
	if (!IsActive() || IsInvincible() || damage <= 0) return;
	hp -= damage;
	if (hp < 0) hp = 0;
	invincibleTimer = PLAYER_INVINCIBLE_TIME;
}

void Player::SetHP(float newHP) { hp = newHP; }
void Player::SetMaxHP(float newMaxHP) { maxHP = newMaxHP; }

float Player::GetHP() const { return hp; }
float Player::GetMaxHP() const { return maxHP; }
float Player::GetInvincibleTimer() const { return invincibleTimer; }
bool Player::IsInvincible() const { return invincibleTimer > 0; }
