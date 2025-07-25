#pragma once

#include "Airplane.h"

class Enemy : public Airplane
{
	using Super = Airplane;
public:
	Enemy(Vector pos, string textureName);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	virtual RenderLayer GetRenderLayer() { return RenderLayer::Enemy; }
	virtual ActorType GetActorType() { return ActorType::Enemy; }
	virtual class ColliderCircle* GetCollider() override { return _circleComponent; }

	virtual void OnEnterCollision(ColliderCircle* other) override;
	virtual void OnExitCollision(ColliderCircle* other) override;
	virtual void OnOverlapCollision(ColliderCircle* other) override;

private:
	class ColliderCircle* _circleComponent = nullptr;

	float _sumRadian = 0;	// 각도값 증가
	float _turnSpeed = 2;	// sin 성장 속도
	float _speedY= 100;
	float _xDir = 1.f;

	float _sumTime = 0;
	float _fireDelay = 1.f;
};

