#include "pch.h"
#include "CollisionManager.h"
#include "Game.h"
#include "GameScene.h"
#include "Actor.h"
#include "ColliderCircle.h"
#include "Scene.h"
#include "Camera.h"

void CollisionManager::Init()
{
	// 플레이어는 본인에 대한 충돌체크를 무시한다.
	SetIgnoreFlag(ActorType::Player, ActorType::Player);
	// 플레이어와 플레이어의 총알에 대해서도 충돌체크를 무시한다.
	SetIgnoreFlag(ActorType::Player, ActorType::PlayerBullet);
}

void CollisionManager::Update(float deltaTime)
{
	// 예전 버전
	//oldCheck();

	// 새로운 버전 : 그리드를 활용
	// 플레이어, 플레이어의 총알만 검사를 수행
 
	
	// 충돌체크가 필요한 주체들만 모아서 검사한다.
	for (auto collider : _checkCollider)
	{
		Cell cell = Cell::ConvertToCell(collider->GetPos(), GRID_SIZE);
		
		// 현재 셀 기준으로 인접한 셀들을 모두 충돌체크 
		for (int32 i = -1; i <= 1; ++i)	// x
		{
			for (int32 j = -1; j <= 1; ++j) // y
			{
				// 인접한 셀의 인덱스 계산
				Cell checkCell{ cell.index_X + i, cell.index_Y + j };	

				// 인접한 Cell 이 관리하고 있는 Actor N개가 필요하다.
				const CellInfo& cellInfo = Game::GetInstance()->GetCurrScene()->GetCellInfo(checkCell);

				// 인접한 셀의 Actor 순회 
				for (const auto& otherActor : cellInfo.actorList)
				{
					ColliderCircle* collider1 = collider;	// 주체
					ColliderCircle* collider2 = otherActor->GetCollider();	// 대상

					// ActorType을 통해서 충돌체크 관계를 파악한다.
					int8 otherActorType = (int8)otherActor->GetActorType();
					int8 colliderActorType = (int8)collider->GetOwner()->GetActorType();

					// colldier 1 과 colldier 2 가 충돌체크 안해도 되는 관계라면
					bool ignore = (COLLISION_BIT_MASK_IGNORE[colliderActorType] & ((uint8)1 << otherActorType));
					if (ignore)
					{
						continue;	// 충돌체크 건너띄기
					}

					if (collider1 && collider2)
					{
						// 두개의 콜라이더가 충돌이 되었나?
						if (collider1->CheckCollision(collider2))
						{
							// 충돌체크 성공
							// OnColisionEnter()
							// OnComponentBeginOverlap()
							addCollisionState(collider1, collider2);
						}
						else
						{
							removeCollisionState(collider1, collider2);
						}
					}
				}
			}
		}
	}
}

// 충돌 체크가 되는 영역
void CollisionManager::Render(HDC hdc)
{
	if (Scene::drawDebugCell)
	{
		// 펜 생성
		HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
		HPEN oldPen = (HPEN)SelectObject(hdc, myPen);

		// 충돌 체크가 필요한 선만 그리기
		for (auto collider : _checkCollider)
		{
			Cell cell = Cell::ConvertToCell(collider->GetPos(), GRID_SIZE);

			// 인접한 셀 모두 표시
			for (int32 i = -1; i <= 1; ++i)	// x
			{
				for (int32 j = -1; j <= 1; ++j) // y
				{
					Cell checkCell{ cell.index_X + i, cell.index_Y + j };

					// 사각형 그리기
					int32 x = checkCell.index_X * GRID_SIZE;
					int32 y = checkCell.index_Y * GRID_SIZE;

					Vector convertPos = Camera::ConvertScreenPos(Vector(x, y));
					x = convertPos.x;
					y = convertPos.y;

					{
						MoveToEx(hdc, x, y, nullptr);
						LineTo(hdc, x + GRID_SIZE, y);
					}
					{
						MoveToEx(hdc, x + GRID_SIZE, y, nullptr);
						LineTo(hdc, x + GRID_SIZE, y + GRID_SIZE);
					}
					{
						MoveToEx(hdc, x + GRID_SIZE, y + GRID_SIZE, nullptr);
						LineTo(hdc, x, y + GRID_SIZE);
					}
					{
						MoveToEx(hdc, x, y + GRID_SIZE, nullptr);
						LineTo(hdc, x, y);
					}
				}
			}
		}

		// 이전 펜 복원 및 새 펜 삭제
		SelectObject(hdc, oldPen);
		DeleteObject(myPen);
	}
}

void CollisionManager::AddCheckCollider(ColliderCircle* collider)
{
	_checkCollider.insert(collider);
}

void CollisionManager::RemoveCheckCollider(ColliderCircle* collider)
{
	_checkCollider.erase(collider);
}

void CollisionManager::addCollisionState(ColliderCircle* src, ColliderCircle* other)
{
	if (src == nullptr || other == nullptr)
		return;

	// src와 other를 묶어서 tuple 생성
	auto collisionPair = std::make_tuple(src, other);

	// 이미 충돌 상태인지 확인
	if (_collisionMap.find(collisionPair) == _collisionMap.end())
	{
		// 충돌 상태 추가
		_collisionMap.insert(collisionPair);

		// 충돌 시작 이벤트 호출
		src->GetOwner()->OnEnterCollision(other);
		other->GetOwner()->OnEnterCollision(src);

		src->OnEnterCollision(src, other);
		other->OnEnterCollision(other, src);
	}
	else
	{
		// 충돌 중 이벤트 호출
		src->GetOwner()->OnOverlapCollision(other);
		other->GetOwner()->OnOverlapCollision(src);

		src->OnOverlapCollision(src, other);
		other->OnOverlapCollision(other, src);
	}
}

void CollisionManager::removeCollisionState(ColliderCircle* src, ColliderCircle* other)
{
	if (src == nullptr || other == nullptr)
		return;

	// src와 other를 묶어서 tuple 생성
	auto collisionPair = std::make_tuple(src, other);

	// 충돌 상태에서 제거
	auto iter = _collisionMap.find(collisionPair);

	// 기존에 충돌된 상태였으면 iter 반환
	if (iter != _collisionMap.end())
	{
		_collisionMap.erase(iter);

		// 충돌 종료 이벤트 호출
		src->GetOwner()->OnExitCollision(other);
		other->GetOwner()->OnExitCollision(src);

		src->OnExitCollision(src, other);
		other->OnExitCollision(other, src);
	}
}

void CollisionManager::oldCheck()
{
	// 내 플레이어 vs 적 총알
	const auto& player = Game::GetInstance()->GetCurrScene()->GetRenderList(RenderLayer::Player);
	const auto& enemyBullet = Game::GetInstance()->GetCurrScene()->GetRenderList(RenderLayer::EnmeyBullet);

	for (auto actor : player)	// 1개
	{
		for (auto bullet : enemyBullet)
		{
			// 콜라이더 정보가 필요해졌다.
			//iter->GetComponents<ColliderCircle>();
			//dynamic_cast<ColliderCircle>() : 매 프레임 dynamic_cast 시도하는건 부담된다.

			// 현실적으로 쉬운방법, actor에 virtual 함수를 선언
			ColliderCircle* collider1 = actor->GetCollider();
			ColliderCircle* collider2 = bullet->GetCollider();

			if (collider1 && collider2)
			{
				// 두개의 콜라이더가 충돌이 되었나?
				if (collider1->CheckCollision(collider2))
				{
					// 충돌체크 성공
					// OnColisionEnter()
					// OnComponentBeginOverlap()
					addCollisionState(collider1, collider2);
				}
				else
				{
					removeCollisionState(collider1, collider2);
				}
			}
		}
	}


	// 적 비행기 vs 내 총알 
	const auto& enmey = Game::GetInstance()->GetCurrScene()->GetRenderList(RenderLayer::Enemy);
	const auto& playerBullet = Game::GetInstance()->GetCurrScene()->GetRenderList(RenderLayer::PlayerBullet);
	for (auto actor : enmey)	// 1개
	{
		for (auto bullet : playerBullet)
		{
			// 콜라이더 정보가 필요해졌다.
			//iter->GetComponents<ColliderCircle>();
			//dynamic_cast<ColliderCircle>() : 매 프레임 dynamic_cast 시도하는건 부담된다.

			// 현실적으로 쉬운방법, actor에 virtual 함수를 선언
			ColliderCircle* collider1 = actor->GetCollider();
			ColliderCircle* collider2 = bullet->GetCollider();

			if (collider1 && collider2)
			{
				// 두개의 콜라이더가 충돌이 되었나?
				if (collider1->CheckCollision(collider2))
				{
					// 충돌체크 성공
					// OnColisionEnter()
					// OnComponentBeginOverlap()
					addCollisionState(collider1, collider2);
				}
				else
				{
					removeCollisionState(collider1, collider2);
				}
			}
		}
	}

	// 내 플레이어 vs 적 비행기
	for (auto actor : player)	// 1개
	{
		for (auto other : enmey)
		{
			// 콜라이더 정보가 필요해졌다.
			//iter->GetComponents<ColliderCircle>();
			//dynamic_cast<ColliderCircle>() : 매 프레임 dynamic_cast 시도하는건 부담된다.

			// 현실적으로 쉬운방법, actor에 virtual 함수를 선언
			ColliderCircle* collider1 = actor->GetCollider();
			ColliderCircle* collider2 = other->GetCollider();

			if (collider1 && collider2)
			{
				// 두개의 콜라이더가 충돌이 되었나?
				if (collider1->CheckCollision(collider2))
				{
					// 충돌체크 성공
					// OnColisionEnter()
					// OnComponentBeginOverlap()
					addCollisionState(collider1, collider2);
				}
				else
				{
					removeCollisionState(collider1, collider2);
				}
			}
		}
	}
}
