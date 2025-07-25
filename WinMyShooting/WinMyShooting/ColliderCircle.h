#pragma once

#include "Component.h"

using CollisionFunc = std::function<void(class ColliderCircle*, class ColliderCircle*)>;

class ColliderCircle : public Component
{
	using Super = Component;

public:
	virtual ~ColliderCircle();
	ColliderCircle(class Actor* owner, float radius);

	virtual void InitCompoent() override;
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc, Vector pos) override;

	Vector GetPos();
	float GetRadius() { return _radius; }
	class Actor* GetOwner() { return _owner; }

	// 원vs원 충돌
	bool CheckCollision(ColliderCircle* other);

	// 충돌 이벤트 등록 함수
	void SetEnterCollisionCallback(CollisionFunc callback) { _funcEnterCollision = callback; }
	void SetExitCollisionCallback(CollisionFunc callback) { _funcExitCollision = callback; }
	void SetOverlapCollisionCallback(CollisionFunc callback) { _funcOverlapCollision = callback; }

	// 충돌 이벤트 호출 함수
	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcEnterCollision) _funcEnterCollision(src, other); }
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcExitCollision) _funcExitCollision(src, other); }
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcOverlapCollision) _funcOverlapCollision(src, other); }

	
private:
	class Actor* _owner = nullptr;	// 내가 지금 누구의 Actor 붙어있는 컴포넌트인지
	float _radius = 0;

	CollisionFunc	_funcEnterCollision = nullptr;		// 첫 충돌 성공
	CollisionFunc	_funcExitCollision = nullptr;		// 충돌 해제
	CollisionFunc	_funcOverlapCollision = nullptr;	// 충돌 유지중
};

