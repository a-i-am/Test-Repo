#include "pch.h"
#include "Missile.h"
#include "Game.h"
#include "Enemy.h"
#include "GameScene.h"

void Missile::Init(float posX, float posY, float angle, bool chase)
{
	_pos.x = posX;
	_pos.y = posY;
	_angle = angle;
	_chase = chase;
}

void Missile::Update(float deltaTime)
{
	// Game 쪽에 GetEnemy() 함수가 있었는데,
	// GameScene 하위로 이사를 갔다.
	GameScene* gameScene = GameScene::GetGameScene();
	if (gameScene == nullptr)
		return;

	if (_chase)
	{
		// 몬스터의 위치를 기준으로 날아간다.
		// 위치 정보를 아예 넘겨받아도 되고, 
		// 적이 이동해도 쫓아가는 기능을 만들기 위해 적의 객체를 가져와서 위치정보를 가져온다.
		Vector targetPos = gameScene->GetEnemy()->GetPos();
		Vector dir = targetPos - _pos;
		dir.Normalize();

		Vector move = dir * _speed * deltaTime;
		_pos += move;
	}
	else
	{
		// cos, sin 각도도 날아가는거 말고
		_pos.x += (_speed * ::cos(_angle) * deltaTime);
		_pos.y -= (_speed * ::sin(_angle) * deltaTime);
	}

	// 화면 밖을 벗어났는지 확인후 소멸
	// 본인의 할일은 본인이 알아서 로직을 처리하자.
	if (_pos.x < 0 || _pos.x >= GWinSizeX || _pos.y < 0 || _pos.y >= GWinSizeY)
	{
		// this 객체가 delete 소멸시키고 나서 더이상 로직 처리 못하게 냅다 리턴하고 ㅌㅌ
		gameScene->RemoveMissile(this);
		return;
	}

	// 다양한 업데이트가 있을수도 있지만.
}

void Missile::Render(HDC hdc)
{
	Ellipse(hdc, _pos.x - 10, _pos.y - 10, _pos.x + 10, _pos.y + 10);
}
