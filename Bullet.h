#pragma once

#include "Entity.h"

class Bullet : public Entity
{
private:
	float damage;

public:
	Bullet();

	void Reset();
	void SetDamage(float newDamage);
	float GetDamage() const;
};
