#pragma once

#include "Actor.h"

class Effect : public Actor
{
	using Super = Actor;
public:
	Effect(Vector pos, string textureKey, float dur);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	virtual RenderLayer GetRenderLayer(){ return RenderLayer::Effect; }

private:
	class SpriteRenderer* _spriteComponent = nullptr;
};