#pragma once

#include "UIManager.h"

// 각 장면을 관리한다.
// LobbyScene, EditScene, GameScene, ChracterSelectScene
class Scene
{
public:
	virtual void Init();
	virtual void Destroy();
	virtual void Update(float deltatTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hdc);


	// Actor Remove
	void ReserveRemove(class Actor* actor);
	void ReserveAdd(class Actor* actor);

	void RemoveAllActor();

protected:
	virtual void loadResource() {}

private:
	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);

protected:
	// 로비씬에서도 액터개념으로 뭔가 그림이 그려져야할수도 있어서
	unordered_set<Actor*> _actors;	// 원본 객체가 있는 개념

	// 렌더링 순서를 보장해서 그리기 위해
	unordered_set<class Actor*> _renderList[(int32)RenderLayer::Count];

	// 추가 예정인 액터들을 관리
	vector<Actor*> _reserveAdd;		// 추가되어야 하는 액터들을 관리
	vector<Actor*> _reserveRemove;	// 삭제되어야 하는 액터들을 관리

	UIManager _ui;
};

