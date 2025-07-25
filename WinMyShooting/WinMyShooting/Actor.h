#pragma once

class Component;
class ObjectPool;

class Actor
{
public:
	Actor(Vector pos);
	virtual ~Actor();

	virtual void Init();
	virtual void Destroy();

	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	virtual RenderLayer GetRenderLayer() { return RenderLayer::Default; }
	virtual ActorType GetActorType() { return ActorType::None; }
	ObjectPool* GetObjectPool() { return _fromObjectPool; }
	void SetObjectPool(ObjectPool* pool) { _fromObjectPool = pool; }

	Vector GetPos() { return _pos; }
	void SetPos(Vector newPos);
	void RemoveFromScene();

	// 본인이 충돌체크를 해야하는 대상이면 CollisionManager 에 등록한다.
	virtual bool IsCheckCollision() { return false; }
	virtual class ColliderCircle* GetCollider() { return nullptr; }

	virtual void OnEnterCollision(ColliderCircle* other) {}
	virtual void OnExitCollision(ColliderCircle* other) {}
	virtual void OnOverlapCollision(ColliderCircle* other) {}

protected:
	class ImageRenderer* CreateImageComponent(string textureKey);
	class SpriteRenderer* CreateSpriteComponent(string textureKey, float dur);
	class ColliderCircle* CreateColliderCircleComponent(float radius);

	// 위치값이 바뀔때마다 그리드를 업데이트 해야한다.
	// 자식들이 알아서 챙겨서 그리드를 업데이트 해야하는데, 버그 날 확률이 무조건 생긴다.
	// 하나의 함수로 통일. 위치가 변경될 일이 있으면 무조건 SetPos() 이 함수를 사용해야 한다.
	// SetPos() 함수를 사용하세요. 의사 소통이 되도, 안쓰는 경우가 생깁니다.
	// 정책을 아예 강제로 SetPos() 사용할수밖에 없게 수정
//protected:
private:
	Vector _pos;				// 씬에 그려져야한다면 위치는 무조건 있어야 한다.

	// 컴포넌트는 1개가 아닐수있다.
	vector<Component*> _components;

	// Actor가 풀에서 태어났으면 그 풀정보를 저장하고 있자.
	ObjectPool*			_fromObjectPool = nullptr;
};

