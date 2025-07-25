#pragma once

#include "Scene.h"

struct EditorActorInfo
{
	// 마우스를 따라다니면서 임시로 그려지고있는 오브젝트
	class Actor* tempActor = nullptr;
	// Sprite Index 값이 필요하다. 마우스 휠을 하면, 다음 Sprite, 이전 Sprite 
	int32 spriteIndex = 0;
};

class EditorScene : public Scene
{
	using Super = Scene;
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltatTime) override;
	virtual void Render(HDC hdc) override;

	void OnMouseWheel(int32 delta);

	void Save();
	void Load();

protected:
	virtual void loadResource();

private:
	enum class SELECT_MODE
	{
		BLOCK,	// 블럭 편집?
		STAR,	// 별을 편집?
		BALL,	// 공을 편집?
		COUNT
	};

	// 현재 어떤 오브젝트를 편집중인지 나타내는 변수.
	SELECT_MODE _currMode = SELECT_MODE::BLOCK;
	EditorActorInfo _editActorInfo[(int32)SELECT_MODE::COUNT];
};