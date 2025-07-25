#pragma once

// 각 장면을 관리한다.
// LobbyScene, EditScene, GameScene, ChracterSelectScene
class Scene
{
public:
	virtual void Init() {}
	virtual void Destroy() {};
	virtual void Update(float deltatTime) {}
	virtual void Render(HDC hdc) {}
};

