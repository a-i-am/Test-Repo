#pragma once

#include "Component.h"

class RectCollider : public Component
{
public:
	RectCollider(class Actor* owner, float width, float height);
	virtual ~RectCollider();

	void InitComponent() override;
	void UpdateComponent(float deltaTime) override;
	void RenderComponent(HDC hdc, Vector pos) override;

	RECT* GetCollisionRect() { return &_collision; }

private:
	class Actor* _owner = nullptr;
	RECT _collision = {}; // 충돌 영역
	float _width = 0; // 너비
	float _height = 0; // 높이
};

