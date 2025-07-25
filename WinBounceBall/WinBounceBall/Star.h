#pragma once

#include "Actor.h"

class Star : public Actor
{
	using Super = Actor;

public:
	Star(Vector pos);
	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;
	RenderLayer GetRenderLayer() override;
	ActorType GetActorType() override { return ActorType::AT_STAR; }
};

