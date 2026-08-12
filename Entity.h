#pragma once

#include "AssetsManager.h"
#include <raylib.h>

class Entity
{
private:
	Vector2 direction;
	Vector2 position;
	float speed;
	float radius;
	bool isActive;
	bool looking;
	TextureName texture;

public:
	Entity();

	void Reset();
	void UpdatePosition();
	void Delete();

	void SetDirection(Vector2);
	void SetPosition(Vector2);
	void SetSpeed(float);
	void SetRadius(float);
	void SetActive(bool);
	void SetLooking(bool);
	void SetTextureName(TextureName);

	Vector2 GetDirection() const;
	Vector2 GetPosition() const;
	float GetSpeed() const;
	float GetRadius() const;
	bool IsActive() const;
	bool GetLooking() const;
	TextureName GetTextureName() const;
};
