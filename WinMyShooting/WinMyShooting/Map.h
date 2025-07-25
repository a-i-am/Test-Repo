#pragma once

#include "Actor.h"

class Map : public Actor
{
	using Super = Actor;
public:
	Map(Vector pos);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	virtual RenderLayer GetRenderLayer() { return RenderLayer::Background; }

private:
	float _moveSpeed = 300;
	Vector _pos2;	// 두번째 백업용으로 그려져야하는 위치
	class ImageRenderer* _textureComponent = nullptr;
};

