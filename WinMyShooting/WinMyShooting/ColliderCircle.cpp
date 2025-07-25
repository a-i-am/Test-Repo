#include "pch.h"
#include "ColliderCircle.h"
#include "Actor.h"

ColliderCircle::~ColliderCircle()
{
}

ColliderCircle::ColliderCircle(class Actor* owner, float radius) : _owner(owner), _radius(radius)
{
}

void ColliderCircle::InitCompoent()
{
}

void ColliderCircle::UpdateComponent(float deltaTime)
{
    // 액터의 크기가 실시간으로 변경된다면, 원 콜라이더도 맞춰서 Radius 값을 갱신해준다.
    //_owner->GetRadius()
}

void ColliderCircle::RenderComponent(HDC hdc, Vector pos)
{
}

Vector ColliderCircle::GetPos()
{
	if (_owner)
		return _owner->GetPos();

	return Vector(0, 0);
}

// 원 vs 원 충돌이서 계산이 비용이 저렴하다.
// 원 vs 원이 아니라, 복잡한 어떠한 폴리곤이었다??
// 플레이어 vs 플레이어 총, 플레이어 vs 플레이어 일 경우에는 애초에 충돌체크를 수행하지 않는다.
bool ColliderCircle::CheckCollision(ColliderCircle* other)
{
	// 중심점끼리의 거리를 계산해서 각 원의 반지름을 합한것보다 크다면 충돌X
    float dx = GetPos().x - other->GetPos().x;
    float dy = GetPos().y - other->GetPos().y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < GetRadius() + other->GetRadius())
    {
        return true;
    }

    // 루트 안씌운 버전이 성능은 좀더 좋다
    //float distanceSquared = dx * dx + dy * dy;
    //float radiusSum = GetRadius() + other->GetRadius();
    //return distanceSquared <= radiusSum * radiusSum;

    return false;
}
