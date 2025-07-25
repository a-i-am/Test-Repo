#include "pch.h"
#include "Effect.h"
#include "SpriteRenderer.h"

Effect::Effect(Vector pos, string textureKey, float dur) : Super(pos)
{
	// Component »ý¼º
	_spriteComponent = CreateSpriteComponent(textureKey, dur);
}

void Effect::Init()
{
	Super::Init();
}

void Effect::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (_spriteComponent)
	{
		if (_spriteComponent->IsEnd())
		{
			RemoveFromScene();
		}
	}
}

void Effect::Render(HDC hdc)
{
	Super::Render(hdc);
}
