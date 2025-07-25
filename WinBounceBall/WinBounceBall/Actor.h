#pragma once

class Component;

enum ActorType : int8
{
	AT_BLOCK,
	AT_BALL,
	AT_STAR,
	AT_EFFECT,
	AT_BACKGROUND,

};

class Actor
{
public:
	Actor(Vector pos);
	virtual ~Actor();

	virtual void Init();
	virtual void Destroy();

	virtual void Update(float deltaTime);
	virtual void Render(HDC renderTarget);
	
	virtual RenderLayer GetRenderLayer() abstract;
	virtual ActorType GetActorType() abstract;

	virtual bool OnBeginOverlapActor(Actor* other) { return false; }
	virtual bool OnEndOverlapActor(Actor* other) { return false; }

	void RemoveFromScene();

	Vector GetPos() { return _pos; }
	void SetPos(Vector pos, bool notifyScene = true);

	virtual void SaveActor(std::wofstream& file);
	virtual void LoadActor(std::wistringstream& steam);

	class RectCollider* GetCollider();
	
	template<typename T>
	T* GetComponent()
	{
		for (auto iter : _components)
		{
			if (T* find = dynamic_cast<T*>(iter))
			{
				return find;
			}
		}
		return nullptr;
	}

protected:

	// Actor 
	class SpriteRenderer* CreateSpriteComponent(string spriteInfo, int32 width = 0, int32 height = 0);
	class ImageRenderer* CreateTextureComponent(string bitmapKey, int32 width = 0, int32 height = 0);
	class RectCollider* CreateRectCollider(float width, float height);

private:
	Vector _pos = {};

	vector<Component*> _components;
};