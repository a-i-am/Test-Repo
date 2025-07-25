#include "pch.h"
#include "Actor.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameScene.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "ColliderCircle.h"
#include "Scene.h"
#include "ImageRenderer.h"

Actor::Actor(Vector pos) : _pos(pos)
{
	
}

Actor::~Actor()
{
}

void Actor::Init()
{
	// 최초 생성시 그리드 정보 갱신
	Game::GetInstance()->GetCurrScene()->UpdateGrid(this, Vector(-1,-1), _pos);
}

void Actor::Destroy()
{
	for (auto iter : _components)
	{
		SAFE_DELETE(iter);
	}
	_components.clear();
}

void Actor::Update(float deltaTime)
{
	for (auto iter : _components)
	{
		iter->UpdateComponent(deltaTime);
	}
}

void Actor::Render(HDC hdc)
{
	for (auto iter : _components)
	{
		iter->RenderComponent(hdc, _pos);
	}
}

// 위치를 변경하고 싶으면 무조건 이 함수를 사용할수 밖에 없다.
void Actor::SetPos(Vector newPos)
{
	Vector prevPos = _pos;
	_pos = newPos;

	// 그리드를 업데이트
	// Scene에 알려준다.
	Game::GetInstance()->GetCurrScene()->UpdateGrid(this, prevPos, newPos);
}

void Actor::RemoveFromScene()
{
	Game::GetInstance()->GetCurrScene()->ReserveRemove(this);
}

ImageRenderer* Actor::CreateImageComponent(string textureKey)
{
	ImageRenderer* imageRenderer = new ImageRenderer(textureKey);
	imageRenderer->InitCompoent();

	_components.push_back(imageRenderer);
	return imageRenderer;
}

SpriteRenderer* Actor::CreateSpriteComponent(string textureKey, float dur)
{
	SpriteRenderer* spriteRenderer = new SpriteRenderer(textureKey, dur);
	spriteRenderer->InitCompoent();

	_components.push_back(spriteRenderer);
	return spriteRenderer;
}

ColliderCircle* Actor::CreateColliderCircleComponent(float radius)
{
	ColliderCircle* circle = new ColliderCircle(this, radius);
	circle->InitCompoent();

	_components.push_back(circle);

	return circle;
}
