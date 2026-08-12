#include "Bullet.h"

Bullet::Bullet() { Reset(); }

void Bullet::Reset()
{
	Entity::Reset();
	damage = 0;
}

void Bullet::SetDamage(float newDamage) { damage = newDamage; }
float Bullet::GetDamage() const { return damage; }
