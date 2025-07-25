#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltatTime) override;
	virtual void Render(HDC hdc) override;


	static GameScene* GetGameScene();

public:
	void CreateMissile(float posX, float posY, float angle, bool chase);
	void RemoveMissile(class Missile* missile);

	class Enemy* GetEnemy() { return _enemy; }


	// 게임이 진행줄일떄 필요한 객체
private:
	// 포인터로 안하고, 객체 그대로 선언해도 된다.
	class Player* _player = nullptr;

	// vector? 뭘로 하지? 고민이 될거에요. 미리 고민하면 답이 없어요.
	// 나중에 기능 점점 추가되면서, vector로는 한계가 있네.
	vector<class Missile*>	_missile;

	class Enemy* _enemy = nullptr;
};

