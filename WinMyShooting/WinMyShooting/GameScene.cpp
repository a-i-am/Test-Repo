#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Game.h"
#include "Bullet.h"
#include "ResourceManager.h"
#include "Map.h"
#include "Enemy.h"
#include "TimeManager.h"
#include "Effect.h"
#include "InputManager.h"
#include "DataManager.h"
#include "ResourceData.h"
#include "Camera.h"
#include "Actor.h"
#include "FixedMap.h"

void GameScene::Init()
{
	Super::Init();

	// 풀의 초기화
	_EnemyBulletPool.Init<EnemyBullet>(50);
	_PlayerBulletPool.Init<PlayerBullet>(50);
}

void GameScene::Destroy()
{
	Super::Destroy();

	// 오브젝트풀의 new 했던 객체도 delete
	_EnemyBulletPool.Clear();
	_PlayerBulletPool.Clear();
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 플레이어의 위치를 카메라의 위치로 판단
	auto playerList = GetRenderList(RenderLayer::Player);
	if (playerList.size() != 0)
	{
		auto iter = playerList.begin();
		Vector playerPos = (*iter)->GetPos();
		Vector mapSize(800, 9700);
		Camera::Update(deltaTime, playerPos, mapSize);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F2))
	{
		Scene::drawDebugCell = !Scene::drawDebugCell;
	}
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);
}

GameScene* GameScene::GetGameScene()
{
	return dynamic_cast<GameScene*>(Game::GetInstance()->GetCurrScene());
}

void GameScene::CreateEnemyMissile(Vector pos)
{
	// 오브젝트풀로 변환
	EnemyBullet* bullet = _EnemyBulletPool.Acquire<EnemyBullet>();

	// 오브젝트 풀 사용시 주의
	// 재사용하는 개념이다 보니깐, 얻어왔을때 '초기화' or 반환할때 '초기화'
	bullet->SetPos(pos);

	// Bullet을 예약 시스템에 넣는다.
	_reserveAdd.push_back(bullet);
}

void GameScene::CreatePlayerMissile(Vector pos)
{
	// 오브젝트풀로 변환
 	PlayerBullet* bullet = _PlayerBulletPool.Acquire<PlayerBullet>();

	// 오브젝트 풀 사용시 주의
	// 재사용하는 개념이다 보니깐, 얻어왔을때 '초기화' or 반환할때 '초기화'
	bullet->SetPos(pos);

	// Bullet을 예약 시스템에 넣는다.
	_reserveAdd.push_back(bullet);
}

void GameScene::CreateEffect(Vector pos, string textureKey)
{
	Effect* effect = new Effect(pos, textureKey, 0.5f);

	// Bullet을 예약 시스템에 넣는다.
	_reserveAdd.push_back(effect);
}

void GameScene::loadResource()
{
	// 데이터로 로드한 모든 텍스쳐들을 로딩한다.
	const ResourceData* resourceData = DataManager::GetInstance()->GetData<ResourceData>(ResourceData::Key());
	if (resourceData == nullptr)
		return;

	for (auto iter : resourceData->_gameSceneData)
	{
		const ResourceData::Item* item = iter.second;

		ResourceManager::GetInstance()->LoadTexture(item->key, item->fileName, item->transparent, item->countX, item->countY);
	}

	// 씬마다 필요한 텍스처를 정해서 로딩한다.
	/*
	ResourceManager::GetInstance()->LoadTexture("BG", L"BG.bmp", -1);
	ResourceManager::GetInstance()->LoadTexture("Player", L"Player.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadTexture("Enemy1", L"Enemy1.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture("Enemy2", L"Enemy2.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture("Enemy3", L"Enemy3.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture("Enemy4", L"Enemy4.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture("PlayerBullet", L"PlayerBullet.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadTexture("PlayerHP", L"PlayerHP.bmp", RGB(252, 0, 255));

	ResourceManager::GetInstance()->LoadTexture("EnemyBullet", L"EnemyBullet.bmp", -1, 5, 1);
	ResourceManager::GetInstance()->LoadTexture("Explosion", L"explosion.bmp", RGB(0, 0, 0), 6, 2);
	ResourceManager::GetInstance()->LoadTexture("Effect_Crash", L"Effect_Crash.bmp", RGB(0, 0, 0), 3, 1);
	*/
}

void GameScene::createBackgroundObject()
{
	// 맵
	//Map* map = new Map(Vector(GWinSizeX / 2, GWinSizeY / 2));
	
	{
		FixedMap* map = new FixedMap(Vector(0, 0));
		addActor(map);

		_mapSizeX = map->GetTextureWidth();
		_mapSizeY = map->GetTextureHeight();

		map->SetPos(Vector(_mapSizeX / 2, _mapSizeY /2));
	}
}

// 초기화에 필요한 오브젝트 생성
void GameScene::createDefaultObject()
{
	// 플레이어 생성
	//Vector initPos = Vector(GWinSizeX / 2, GWinSizeY - 200);
	Vector initPos = Vector(GetMapSizeX() / 2, GetMapSizeY() - 200);
	Player* player = new Player(initPos);
	addActor(player);
	Camera::SetCameraPos(initPos);
	//_player = player;

	// 등장하는 몬스터
	createRandomEnemy();

	// 반복해서 몬스터를 스폰하는 타이머
	TimeManager::GetInstance()->AddTimer([]()
		{
			GameScene::GetGameScene()->createRandomEnemy();
		},
		2.0f, true);
}

void GameScene::createRandomEnemy()
{
	string textureKey[4] = { "Enemy1", "Enemy2", "Enemy3", "Enemy4" };
	int randomIndex = rand() % 4;
	const int32 enemyCount = 4;

	Vector pos{ 50, 100 };
	int32 xDelta = GWinSizeX / enemyCount;

	// 4종류의 텍스처 중에서 랜덤한 적 생성
	for (int32 i = 0; i < enemyCount; ++i)
	{
		Vector initPos = pos;
		initPos.x += (xDelta * i);

		Enemy* enemy = new Enemy(initPos, textureKey[randomIndex]);
		addActor(enemy);
	}
}