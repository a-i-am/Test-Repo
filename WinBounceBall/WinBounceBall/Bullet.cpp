#include "pch.h"
#include "Bullet.h"
#include "Texture.h"
#include "ResourceManager.h"

void Bullet::Init(Vector pos)
{
	_pos = pos;

	//_texture = ResourceManager::GetInstance()->GetTexture("PlayerBullet");
}

void Bullet::Update(float deltaTime)
{
	_pos.y -= _speed * deltaTime;
}

void Bullet::Render(HDC hdc)
{
	//if (_texture)
	//{
	//	_texture->Render(hdc, _pos);
	//}
}
