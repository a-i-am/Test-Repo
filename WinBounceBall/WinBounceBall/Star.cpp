#include "pch.h"
#include "Star.h"

Star::Star(Vector pos) : Super(pos)
{
}

void Star::Init()
{
    Super::Init();

    SpriteRenderer* renderer = CreateSpriteComponent("Star_0", STAR_SIZE, STAR_SIZE);
    CreateRectCollider(STAR_SIZE, STAR_SIZE);
}

void Star::Update(float deltaTime)
{
    Super::Update(deltaTime);
}

void Star::Render(HDC renderTarget)
{
    Super::Render(renderTarget);
}

RenderLayer Star::GetRenderLayer()
{
    return RenderLayer::Object;
}
