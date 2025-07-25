#pragma once

// Actor에 붙어서, 본인의 기능에 맞게 Update, Render 담당
class Component
{
public:
	virtual ~Component() {}

	virtual void InitCompoent() {}
	virtual void UpdateComponent(float deltaTime) {}
	virtual void RenderComponent(HDC hdc, Vector pos) {}
};

