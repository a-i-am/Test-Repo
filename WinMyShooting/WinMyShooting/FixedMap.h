#pragma once

#include "Actor.h"

// 고정해서 가만히 있는 맵 배경
class FixedMap : public Actor
{
	using Super = Actor;

public:
	FixedMap(Vector pos);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	virtual RenderLayer GetRenderLayer() { return RenderLayer::Background; }

	int32 GetTextureWidth();
	int32 GetTextureHeight();
private:
	class ImageRenderer* _textureComponent = nullptr;
};

