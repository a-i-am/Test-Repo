#include "pch.h"
#include "Enemy.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameScene.h"
#include "Texture.h"
#include "ColliderCircle.h"
#include "Bullet.h"
#include "ImageRenderer.h"

Enemy::Enemy(Vector pos, string textureName) : Super(pos)
{
	_speed = 50;
	_speedY = 50;

	ImageRenderer* texture = CreateImageComponent(textureName);

	// 충돌체 추가
	_circleComponent = CreateColliderCircleComponent((float)texture->GetTextureWidth() / 2);
}

void Enemy::Init()
{
	Super::Init();
}

void Enemy::Update(float deltaTime)
{
	Super::Update(deltaTime);

	float x = _speed * deltaTime * sinf(_sumRadian);
	//float x = _speed * _xDir * deltaTime;
	float y = _speedY * deltaTime;

	Vector newPos = GetPos();
	_sumRadian += (_turnSpeed * deltaTime);
	newPos.x += x;
	newPos.y += y;
	SetPos(newPos);

	// 양끝에 부딪히면 방향 전환
	//if (_pos.x > GWinSizeX)
	//{
	//	_xDir = -1.f;
	//}
	//if (_pos.x < 0)
	//{
	//	_xDir = 1.f;
	//}

	_sumTime += deltaTime;

	// 일정 시간으로 총알 발사
	if (_sumTime >= _fireDelay)
	{
		// 총알 발사
		GameScene::GetGameScene()->CreateEnemyMissile(GetPos());

		_sumTime -= _fireDelay;
	}


	// 화면밖으로 나갔으면 삭제 요청
	if (GetPos().y > Game::GetInstance()->GetCurrScene()->GetMapSizeY())
	{
		RemoveFromScene();
	}

	// 그리드 업데이트 호출
}

void Enemy::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Enemy::OnEnterCollision(ColliderCircle* other)
{
	// 콜백을 받아서 Enemy 한테 충돌 체크 성공시 호출
	PlayerBullet* bullet = dynamic_cast<PlayerBullet*>(other->GetOwner());
	if (bullet)
	{
		// 플레이어의 총알과 부딪혀서 사라진다.
		RemoveFromScene();

		// 터지는 이펙트 재생
		GameScene::GetGameScene()->CreateEffect(GetPos(), "Explosion");

		// 스코어도 증가
	}
}

void Enemy::OnExitCollision(ColliderCircle* other)
{
}

void Enemy::OnOverlapCollision(ColliderCircle* other)
{
}
