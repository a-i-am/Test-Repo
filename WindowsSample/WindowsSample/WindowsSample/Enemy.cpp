#include "pch.h"
#include "Enemy.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "TextureResource.h"

void Enemy::Init()
{
	_pos.x = GWinSizeX / 2;	// 중앙 위치
	_pos.y = 100;

	_start.x = 100;
	_start.y = 100;

	_end.x = 500;
	_end.y = 300;

	_texture = ResourceManager::GetInstance()->GetTexture("Player");
}

void Enemy::Update(float deltaTime)
{
	Vector lineVec = _end - _start;
	float lineDist = lineVec.Length();

	lineVec.Normalize();

	Vector mousePos(InputManager::GetInstance()->GetMousePos());
	Vector dirToMouse = mousePos - _start;

	float dist = lineVec.Dot(dirToMouse);

	if (dist < 0)
		dist = 0;	// 시작 지점으로 고정
	else if (dist > lineDist)
		dist = lineDist;

	// 특정 라인을 따라서 위치가 정해진다.
	_pos = _start + (lineVec * dist);
}

void Enemy::Render(HDC hdc)
{
	// winAPI 텍스처 그릴때, 위치를 기준으로 텍스처를 그리는데, 이때도 똑같이 _pos 는 중심위치로 생각하면
	// 로직만들때 편하다.
	//::Rectangle(hdc, _pos.x - 25, _pos.y - 25, _pos.x + 25, _pos.y + 25);

	if (_texture)
	{
		_texture->Render(hdc, _pos);
	}

	// 벡터 투영이라는걸 배웠고, 길이값이 나온다는데, 도대체 어떻게 동작하는지 모르겠따.

	// 적이 따라서 움직여야되는 라인을 그려보자.
	::MoveToEx(hdc, (int32)_start.x, (int32)_start.y, nullptr);
	::LineTo(hdc, (int32)_end.x, (int32)_end.y);

	Vector lineVec = _end - _start;
	lineVec.Normalize();

	Vector mousePos(InputManager::GetInstance()->GetMousePos());
	Vector dirToMouse = mousePos - _start;

	float dist = lineVec.Dot(dirToMouse);
	wstring str = std::format(L"distance({0})", dist);
	::TextOut(hdc, 300, 300, str.c_str(), static_cast<int32>(str.size()));
}
