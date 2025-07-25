#pragma once

#include "Singleton.h"

class ColliderCircle;

class CollisionManager : public Singleton<CollisionManager>
{
	friend Singleton<CollisionManager>;

private:
	CollisionManager() {}

public:

	void Init();

	// 충돌 검사를 수행
	void Update(float deltaTime);

	void Render(HDC hdc);

	// 충돌 주체가 되어달라고 추가
	void AddCheckCollider(ColliderCircle* collider);

	// 충돌체크를 껏다/켯다 것만으로 기능을 추가 할수 있다.
	// 나의 비행기 3초간 무적이 되는 아이템을 먹었다.
	// 충돌체크를 하지 않으면?? 무적이 되지 않나?
	// 아이템도 못먹고, 아무것도 못하는 상태. (은신 상태)
	void RemoveCheckCollider(ColliderCircle* collider);

	// 1byte : 8bit
	// [7][6][5][4][3][2][1][0]
	// 각 비트의 위치가 : 오브젝트의 타입 (ActorType)
	void SetIgnoreFlag(ActorType type, ActorType ignore)
	{
		// Ex) Player, Player Bullet은 충돌체크 하지 않는다.
		// Player bit
		//COLLISION_BIT_MASK_IGNORE[ActorType::Player]
		// [7][6][5][4][3][2][1][0] = 0
		//  0  0  0  0  0  0  0  0
		//  0  0  0  0  1  0  0  0  :  PlayerBullet (3번)
		// PlayerBullet bit 켜고 싶다.
		// PlayerBullet (3번)

		// 숫자 1의 비트 상태
		// 0  0  0  0  0  0  0  1
		// 0  0  0  0  1  0  0  0    : 1 << 3
		COLLISION_BIT_MASK_IGNORE[(int8)type] |= ((uint8)1 << (int8)ignore);

		// PlayerBullet 대한 비트설정도, Player 비트를 켜서 무시한다.
		COLLISION_BIT_MASK_IGNORE[(int8)ignore] |= ((uint8)1 << (int8)type);
	}

	// 해당 비트( 오브젝트의 타입 ) 
	uint8 COLLISION_BIT_MASK_IGNORE[(int32)ActorType::Max] = {};

private:
	void addCollisionState(ColliderCircle* src, ColliderCircle* other);
	void removeCollisionState(ColliderCircle* src, ColliderCircle* other);

	void oldCheck();
	
private:
	// key : scr Collider, value : other Collider List
	//struct CollisionInfo
	//{
	//	ColliderCircle* src;
	//	ColliderCircle* other;
	//};

	// 충돌체크를 해야하는 콜라이더 모음 : 플레이어 + 플레이어의 총알
	set<ColliderCircle*> _checkCollider;

	// 첫 충돌여부? 충돌 해제? 오버랩 여부
	set<tuple<ColliderCircle*, ColliderCircle*>>	_collisionMap;
};

