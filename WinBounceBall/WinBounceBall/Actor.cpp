#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "ImageRenderer.h"
#include "RectCollider.h"

Actor::Actor(Vector pos)
{
	_pos = pos;
}

Actor::~Actor()
{
	for (auto iter : _components)
	{
		SAFE_DELETE(iter);
	}
	_components.clear();
}

void Actor::Init()
{
	for (auto iter : _components)
	{
		iter->InitComponent();
	}
}

void Actor::Update(float deltaTime)
{
	for (auto iter : _components)
	{
		iter->UpdateComponent(deltaTime);
	}
}

void Actor::Render(HDC renderTarget)
{
	for (auto iter : _components)
	{
		iter->RenderComponent(renderTarget, GetPos());
	}
}

void Actor::Destroy()
{
	for (auto iter : _components)
	{
		SAFE_DELETE(iter);
	}
	_components.clear();
}

void Actor::SetPos(Vector pos, bool notifyScene)
{
	Vector prevPos = _pos;
	_pos = pos;
}

void Actor::RemoveFromScene()
{
	Game::GetInstance()->GetCurrScene()->ReserveRemove(this);
}


void Actor::SaveActor(std::wofstream& file)
{
	SpriteRenderer* sprite = GetComponent<SpriteRenderer>();
	if (nullptr == sprite)
		return;

	int32 indexX = 0, indexY = 0;
	sprite->GetIndex(indexX, indexY);

	Vector pos = GetPos();
	char comma = ','; // 데이터 끼리의 구분자 역할
	// cout <<
	// cin >> 
	file << (int32)GetActorType() << comma;	// 1번째로 액터 타입 저장
	file << indexX << comma << indexY << comma;	// 2번째 Sprite
	file << (int32)pos.x << comma << (int32)pos.y << comma; // 3번째 좌표
	file << std::endl;
}

void Actor::LoadActor(std::wistringstream& steam)
{
	SpriteRenderer* sprite = GetComponent<SpriteRenderer>();
	if (nullptr == sprite)
		return;

	// 이미 액터가 생성되었다는건 해당 file stream 에서 actorType에 대한 값을 읽은 이후이다.
	wchar_t comma = ',';
	steam >> comma;	// GetActorType() 저장이후 구분자 읽기

	int32 indexX = 0, indexY = 0;
	steam >> indexX >> comma >> indexY >> comma;

	int32 x = 0, y = 0;
	steam >> x >> comma >> y >> comma;

	SetPos(Vector((float)x, (float)y), true);
	sprite->SetSpriteIndex(indexX, indexY);
}

RectCollider* Actor::GetCollider()
{
	return GetComponent<RectCollider>();
	//for (auto iter : _components)
	//{
	//	if (RectCollider* collider = dynamic_cast<RectCollider*>(iter))
	//	{
	//		return collider;
	//	}
	//}

	//return nullptr;
}

SpriteRenderer* Actor::CreateSpriteComponent(string spriteInfo, int32 width, int32 height)
{
	SpriteRenderer* sprite = new SpriteRenderer(spriteInfo, width, height);
	_components.emplace_back(sprite);
	return sprite;
}

ImageRenderer* Actor::CreateTextureComponent(string bitmapKey, int32 width, int32 height)
{
	ImageRenderer* sprite = new ImageRenderer(bitmapKey, width, height);
	_components.emplace_back(sprite);
	return sprite;
}

RectCollider* Actor::CreateRectCollider(float width, float height)
{
	RectCollider* collider = new RectCollider(this, width, height);
	_components.emplace_back(collider);
	return collider;
}
