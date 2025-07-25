#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Game.h"


void GameScene::Init()
{
	// 플레이어 생성
	_player = new Player();
	_player->Init();

	// 적 생성
	_enemy = new Enemy();
	_enemy->Init();
}

void GameScene::Destroy()
{
	SAFE_DELETE(_player);
	SAFE_DELETE(_enemy);

	// 화면안에 남아있는 미사일도 정리
	for (auto iter : _missile)
	{
		delete iter;
	}
	_missile.clear();
}

void GameScene::Update(float deltatTime)
{
	if (_player)
	{
		_player->Update(deltatTime);
	}

	if (_enemy)
	{
		_enemy->Update(deltatTime);
	}

	// 플레이어 함수입니다.
	for (auto iter : _missile)
	{
		iter->Update(deltatTime);

		// 좌표를 벗어나면,
		// _missile vector 에서 제거하면서, delete (for문 순회하면서 delete)
	}

	// 오브젝트 풀 - 내가 10개의 공을 가지고 있으면, 공이 필요할때마다 돌려쓰자.(좌표만 변경해서)
}

void GameScene::Render(HDC hdc)
{
	if (_player)
	{
		_player->Render(hdc);
	}

	if (_enemy)
	{
		_enemy->Render(hdc);
	}

	for (auto iter : _missile)
	{
		iter->Render(hdc);
	}
}

GameScene* GameScene::GetGameScene()
{
	return dynamic_cast<GameScene*>(Game::GetInstance()->GetCurrScene());
}

void GameScene::CreateMissile(float posX, float posY, float angle, bool chase)
{
	Missile* missile = new Missile();
	missile->Init(posX, posY, angle, chase);
	_missile.push_back(missile);
}

void GameScene::RemoveMissile(Missile* missile)
{
	// 벡터를 돌면서 missile과 동일한 객체를 찾아야 한다.
	//for ();
	auto it = std::find(_missile.begin(), _missile.end(), missile);
	_missile.erase(it);

	// 객체도 소멸한다.
	// 객체의 생성과 소멸은 모두 Game이라는 매니저가 총괄
	// 이 코드는 괜찮을까요? 위험한 코드를 만들어야 하네.... 나중에 한번 고민해보자.TODO
	delete missile;
}

