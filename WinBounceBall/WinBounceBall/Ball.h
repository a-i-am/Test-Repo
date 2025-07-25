#pragma once

#include "Actor.h"

class Ball : public Actor
{
	using Super = Actor;

public:
	Ball(Vector pos);
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;
	RenderLayer GetRenderLayer() override;
	ActorType GetActorType() override { return ActorType::AT_BALL; }

private:
	void applyPhysics(float deltaTime);

public:
	// 물리 효과가 적용되는 액터. 적용이 안되는 액터
	// RigidBody 컴포넌트를 생성후, 해당 액터에 붙이면, 물리 효과가 적용
	// 이 구조가 조금더 효율적이긴한데, 우리는 구조에 대한 고민을 하는 시간이 아니라서
	// Ball 에다가 바로 구현

	// 중력값은 대략 9.8
	Vector _gravityVec = Vector(0, 980.f);

	Vector velocity = {};       // 공의 속도 (speed 같은 개념)
	Vector acceleration = {};	// 가속도
};

