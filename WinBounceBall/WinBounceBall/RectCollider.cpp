#include "pch.h"
#include "RectCollider.h"
#include "Actor.h"
#include "ResourceManager.h"

RectCollider::RectCollider(Actor* actor, float width, float height)
{
	_owner = actor;
	_width = width;
	_height = height;
}

RectCollider::~RectCollider()
{
}

void RectCollider::InitComponent()
{
}

void RectCollider::UpdateComponent(float deltaTime)
{
	_collision.left = _owner->GetPos().x + _width * -0.5f;
	_collision.right = _owner->GetPos().x + _width * 0.5f;
	_collision.top = _owner->GetPos().y + _height * -0.5f;
	_collision.bottom = _owner->GetPos().y + _height * 0.5f;
}

void RectCollider::RenderComponent(HDC hdc, Vector pos)
{
	return;

	// 충돌 영역을 그리기
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HGDIOBJ oldPen = SelectObject(hdc, pen);

	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldbrush = (HBRUSH)::SelectObject(hdc, brush);

	Rectangle(hdc, (int)_collision.left, (int)_collision.top, (int)_collision.right, (int)_collision.bottom);
	
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	SelectObject(hdc, oldbrush);
	DeleteObject(brush);
}
