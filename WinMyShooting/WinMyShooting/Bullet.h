#pragma once

#include "Actor.h"

class Bullet : public Actor
{
	using Super = Actor;
public:
	Bullet(Vector pos);
			
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual class ColliderCircle* GetCollider() override { return _circleComponent; }

protected:
	class ColliderCircle* _circleComponent = nullptr;

	float _speed = 300;
	float _dirY = 1.f;
};


class PlayerBullet : public Bullet
{
	using Super = Bullet;
public:
	PlayerBullet(Vector pos);
	virtual RenderLayer GetRenderLayer() { return RenderLayer::PlayerBullet; }
	virtual ActorType GetActorType() { return ActorType::PlayerBullet; }

	// 플레이어 총알만 충돌체크 주체
	virtual bool IsCheckCollision() override { return true; }
	virtual void OnEnterCollision(ColliderCircle* other) override;
	virtual void Render(HDC hdc) override;
};

class EnemyBullet : public Bullet
{
	using Super = Bullet;
public:
	EnemyBullet(Vector pos);
	virtual RenderLayer GetRenderLayer() { return RenderLayer::EnmeyBullet; }
	virtual ActorType GetActorType() { return ActorType::EnemyBullet; }
	virtual void OnEnterCollision(ColliderCircle* other) override;
};