#include "pch.h"
#include "GameScene.h"
#include "Game.h"
#include "StageLoader.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Actor.h"
#include "RectCollider.h"

void GameScene::Init()
{
	Super::Init();


	loadStage(1);

	_bg = ResourceManager::GetInstance()->GetTexture("GameBG");
	_bg->SetCenterAlign(false);
}

void GameScene::Destroy()
{
	Super::Destroy();
}

void GameScene::Update(float deltatTime)
{
	Super::Update(deltatTime);


}

void GameScene::Render(HDC hdc)
{
	if (_bg)
	{
		_bg->Render(hdc, Vector(0, 0), Vector(0, 0));
	}

	Super::Render(hdc);
}

GameScene* GameScene::GetGameScene()
{
	return dynamic_cast<GameScene*>(Game::GetInstance()->GetCurrScene());
}

bool GameScene::CheckCollision(Ball* ball, Vector start, Vector end, Vector& outNormal, Vector& outPos)
{
	// 전체 액터를 다 순회하면서, Line, AABB 충돌체크
	// 1 : N

	float tMin = FLT_MAX;
	Actor* closestActor = nullptr;

	for (auto iter : _actors)
	{
		if (!iter->GetCollider())
			continue;

		// actor->GetActorType() vs Ball -> Block
		// actor->GetActorType() vs Ball -> Overlap
		// actor->GetActorType() vs Ball -> Ignore

		RECT rect = *iter->GetCollider()->GetCollisionRect();
		rect.left -= BALL_SIZE * 0.5f;
		rect.right += BALL_SIZE * 0.5f;
		rect.top -= BALL_SIZE * 0.5f;
		rect.bottom += BALL_SIZE * 0.5f;
		
		float t = 0.f;
		Vector normal;
		Vector hitPos;
		if (LineIntersectsAABB(start, end, rect, normal, hitPos, t))
		{
			// 충돌 됨.
			if (t < tMin)
			{
				outNormal = normal;
				outPos = hitPos;
				tMin = t;
				closestActor = iter;

				//iter->OnBeginOverlapActor();
			}
		}
	}

	if (closestActor != nullptr)
	{
		// 공과 무언가가 충돌되었고, overlap 처리
		// closestActor->OnBeginOverlapActor();
		//ball->OnBeginOverlapActor(closestActor);
		return true;
	}

	return false;
}

void GameScene::loadResource()
{
	ResourceManager::GetInstance()->LoadTexture("GameBG", L"background_game.bmp");
	ResourceManager::GetInstance()->LoadTexture("Ball", L"001-008_ball.bmp", 2, 4);
	ResourceManager::GetInstance()->LoadTexture("Block", L"201-217_block.bmp", 3, 6);
	ResourceManager::GetInstance()->LoadTexture("Star", L"101-103_star.bmp", 1, 3);
	ResourceManager::GetInstance()->LoadTexture("EatStarEffect", L"Eat_Star.bmp", 23, 1);
	ResourceManager::GetInstance()->LoadTexture("DeadBall", L"Deadball.bmp", 42, 1);
	ResourceManager::GetInstance()->LoadTexture("LevelComplete", L"level_complete.png");
	ResourceManager::GetInstance()->LoadTexture("NextStageButton", L"next_stage.png");
}

void GameScene::loadStage(int32 stage)
{
	wstring fileName = std::format(L"stage_{0}.stage", stage);
	fs::path fullPath = fs::current_path() / L"../Resources/Stage/" / fileName;

	std::wifstream file(fullPath);
	if (file.is_open())
	{
		StageLoader loader;
		_maxStarCount = loader.Load(this, file);
		_curStarCount = 0;

		file.close();
	}
}
