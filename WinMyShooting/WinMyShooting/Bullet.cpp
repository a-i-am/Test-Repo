#include "pch.h"
#include "Bullet.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "GameScene.h"
#include "Game.h"
#include "SpriteRenderer.h"
#include "ColliderCircle.h"
#include "Enemy.h"
#include "Player.h"
#include "ImageRenderer.h"

Bullet::Bullet(Vector pos) : Super(pos)
{
}

void Bullet::Update(float deltaTime)
{
	Super::Update(deltaTime);

	Vector newPos = GetPos();
	newPos.y += (_speed * deltaTime * _dirY);
	SetPos(newPos);

	// 맵 밖으로 나갔다. 삭제 요청
	if (GetPos().y < 0 || GetPos().y > Game::GetInstance()->GetCurrScene()->GetMapSizeY())
	{
		RemoveFromScene();
	}
}

void Bullet::Render(HDC hdc)
{
	Super::Render(hdc);
}


//------------------------------------------------------------
PlayerBullet::PlayerBullet(Vector pos) : Super(pos)
{
	_dirY = -1.0f;

	// 플레이어 총알
	ImageRenderer* texture = CreateImageComponent("PlayerBullet");
	// 충돌체 추가
	_circleComponent = CreateColliderCircleComponent((float)texture->GetTextureWidth() / 2);
}

void PlayerBullet::OnEnterCollision(ColliderCircle* other)
{
	if (dynamic_cast<Enemy*>(other->GetOwner()))
	{
		RemoveFromScene();
	}
}

void PlayerBullet::Render(HDC hdc)
{
	Super::Render(hdc);
}

//--------------------------------------------------------------
EnemyBullet::EnemyBullet(Vector pos) : Super(pos)
{
	_dirY = 1.0f;

	// 적의 총알
	SpriteRenderer* SpriteRenderer = CreateSpriteComponent("EnemyBullet", 0);
	SpriteRenderer->SetFrameX(rand() % 5);
	// 충돌체 추가
	_circleComponent = CreateColliderCircleComponent((float)SpriteRenderer->GetRenderSizeX() / 2);
}

void EnemyBullet::OnEnterCollision(ColliderCircle* other)
{
	if (dynamic_cast<Player*>(other->GetOwner()))
	{
		RemoveFromScene();
	}
}
