#pragma once

#include "Airplane.h"

class Player : public Airplane
{
	using Super = Airplane;
public:
	Player(Vector pos);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	virtual RenderLayer GetRenderLayer() { return RenderLayer::Player; }
	virtual ActorType GetActorType() { return ActorType::Player; }

	// 플레이어는 충돌체크 주체
	virtual bool IsCheckCollision() override { return true; }

	virtual class ColliderCircle* GetCollider() override { return _circleComponent; }

	virtual void OnEnterCollision(ColliderCircle* other) override;

private:
	void takeDamage();

private:
	class ColliderCircle* _circleComponent = nullptr;
	int32 _hp = 10;
};

