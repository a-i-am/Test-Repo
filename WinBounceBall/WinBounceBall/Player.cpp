#include "pch.h"
#include "Player.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Texture.h"
#include "GameScene.h"

void Player::Init()
{
	_pos.x = GWinSizeX / 2;
	_pos.y = GWinSizeY - 200;
	_texture = ResourceManager::GetInstance()->GetTexture("Player");
}

void Player::Update(float deltaTime)
{
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		// 이동 거리 = 시간 * 속도
		// 1초당 10만큼 이동한다.
		_pos.y -= (deltaTime * _speed);	// 모든 PC가 1프레임당 동일한 거리를 이동한다.
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		_pos.y += (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		_pos.x -= (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		_pos.x += (deltaTime * _speed);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::SpaceBar))
	{
		GameScene::GetGameScene()->CreateMissile(_pos);
	}
}

void Player::Render(HDC hdc)
{
	if (_texture)
	{
		_texture->Render(hdc, _pos);
	}
}
