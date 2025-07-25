#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include "Bullet.h"

//class EnemyBullet;
//class PlayerBullet;

class GameScene : public Scene
{
	using Super = Scene;

public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltatTime) override;
	virtual void Render(HDC hdc) override;


	static GameScene* GetGameScene();

public:
	void CreateEnemyMissile(Vector pos);
	void CreatePlayerMissile(Vector pos);
	void CreateEffect(Vector pos, string textureKey);
	//class Enemy* GetEnemy() { return _enemy; }

protected:
	virtual void loadResource();
	virtual void createBackgroundObject() override;
	virtual void createDefaultObject();
	
	void createRandomEnemy();

	// 게임이 진행줄일떄 필요한 객체
private:
	class Player* _player = nullptr;
	
	// 오브젝트 풀 매니저 등장해도 괜찮다.
	// GameScene 에서 전체 관리

	ObjectPool _EnemyBulletPool;
	ObjectPool _PlayerBulletPool;
};

