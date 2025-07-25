#include "pch.h"
#include "Player.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Texture.h"
#include "GameScene.h"
#include "ColliderCircle.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ImageRenderer.h"
#include "Camera.h"

Player::Player(Vector pos) : Super(pos)
{
}

void Player::Init()
{
	Super::Init();

	ImageRenderer* texture = CreateImageComponent("Player");

	// 충돌체 추가
	_circleComponent = CreateColliderCircleComponent((float)texture->GetTextureWidth() / 2);
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	Vector newPos = GetPos();

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		// 이동 거리 = 시간 * 속도
		// 1초당 10만큼 이동한다.
		newPos.y -= (deltaTime * _speed);	// 모든 PC가 1프레임당 동일한 거리를 이동한다.
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		newPos.y += (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		newPos.x -= (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		newPos.x += (deltaTime * _speed);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::SpaceBar))
	{
		GameScene::GetGameScene()->CreatePlayerMissile(GetPos());
	}

	// 좌상단 (화면위치 기준의 좌표)
	Vector leftTop = Vector(50, 100);
	Vector convertLeftTop = Camera::ConvertWorldPos(leftTop);

	// 우하단 (화면위치 기준의 좌표)
	Vector rightBottom = Vector(GWinSizeX - 50, GWinSizeY - 100);
	Vector convertRightBottom = Camera::ConvertWorldPos(rightBottom);

	//if (newPos.x < convertLeftTop.x)
	//{
	//	newPos.x = convertLeftTop.x;
	//}

	newPos.x = max(newPos.x, convertLeftTop.x);
	newPos.x = min(newPos.x, convertRightBottom.x);

	newPos.y = max(newPos.y, convertLeftTop.y);
	newPos.y = min(newPos.y, convertRightBottom.y);

	SetPos(newPos);
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::OnEnterCollision(ColliderCircle* other)
{
	EnemyBullet* bullet = dynamic_cast<EnemyBullet*>(other->GetOwner());
	if (bullet)
	{
		takeDamage();
	}

	Enemy* enemy = dynamic_cast<Enemy*>(other->GetOwner());
	if (enemy)
	{
		takeDamage();
	}
}

void Player::takeDamage()
{
	// 내가 무적 상태라면 데미지 안깍고 return;

	//--_hp;

	GameScene::GetGameScene()->CreateEffect(GetPos(), "Effect_Crash");

	if (_hp == 0)
	{
		// 터지는 폭발 애니메이션

		// 게임 오버 처리

		// 씬에서 삭제
		RemoveFromScene();
	}
}
