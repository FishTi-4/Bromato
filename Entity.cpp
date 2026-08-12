#include "Entity.h"
#include "RuntimeData.h"
#include <raymath.h>

extern RuntimeData data;

Entity::Entity() { Reset(); }

void Entity::Reset()
{
	direction = { 0,0 };
	position = { 0,0 };
	speed = 0;
	radius = 0;
	isActive = 0;
	looking = 0;
	texture = TextureName::Count;
}

void Entity::UpdatePosition()
{
	if (!isActive) return;
	if (direction.x || direction.y) direction = Vector2Normalize(direction);
	position.x += direction.x * speed * data.dt;
	position.y += direction.y * speed * data.dt;
}

void Entity::Delete() { isActive = 0; }

void Entity::SetDirection(Vector2 newDirection) { direction = newDirection; }
void Entity::SetPosition(Vector2 newPosition) { position = newPosition; }
void Entity::SetSpeed(float newSpeed) { speed = newSpeed; }
void Entity::SetRadius(float newRadius) { radius = newRadius; }
void Entity::SetActive(bool active) { isActive = active; }
void Entity::SetLooking(bool newLooking) { looking = newLooking; }
void Entity::SetTextureName(TextureName newTexture) { texture = newTexture; }

Vector2 Entity::GetDirection() const { return direction; }
Vector2 Entity::GetPosition() const { return position; }
float Entity::GetSpeed() const { return speed; }
float Entity::GetRadius() const { return radius; }
bool Entity::IsActive() const { return isActive; }
bool Entity::GetLooking() const { return looking; }
TextureName Entity::GetTextureName() const { return texture; }
