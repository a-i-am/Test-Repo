#pragma once
#include "Scene.h"

class GameScene : public Scene
{
	using Super = Scene;
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltatTime) override;
	virtual void Render(HDC hdc) override;


	static GameScene* GetGameScene();
	// Start : Old Ball Pos
	// End : New Ball Pos
	bool CheckCollision(class Ball* ball, Vector start, Vector end, Vector& outNormal, Vector& outPos);

protected:
	virtual void loadResource();
	void loadStage(int32 stage);
public:

	// 게임이 진행줄일떄 필요한 객체
private:
	class Texture* _bg = nullptr; // 객체화 시키면 좋은데, 지금은 빠르게 

	int32 _stage = 1;	// 현재 진행중인 스테이지 
	int32 _maxStarCount = 0;	// 먹어야하는 별이 몇개인지
	int32 _curStarCount = 0;	// 현재 먹은 별의 개수
};

