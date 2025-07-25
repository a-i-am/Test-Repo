#pragma once


class Component
{
public:
	virtual ~Component() {}
	virtual void InitComponent() {}
	virtual void UpdateComponent(float deltaTime) {}
	virtual void RenderComponent(HDC hdc, Vector pos) {}
};

