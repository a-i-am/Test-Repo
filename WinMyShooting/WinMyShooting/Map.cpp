#include "pch.h"
#include "Map.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "ImageRenderer.h"

Map::Map(Vector pos) : Super(pos)
{
	_textureComponent = CreateImageComponent("BG");

	// 여분 텍스처의 초기 위치
	_pos2.x = pos.x;
	_pos2.y = -GWinSizeY / 2;
}

void Map::Init()
{
	Super::Init();
}

void Map::Update(float deltaTime)
{
	Super::Update(deltaTime);

	Vector newPos = GetPos();
	// y 축으로 아래로 이동한다.
	newPos.y += _moveSpeed * deltaTime;

	if (newPos.y > GWinSizeY + (GWinSizeY / 2))
	{
		newPos.y -= GWinSizeY * 2.0f;
	}
	SetPos(newPos);

	_pos2.y += _moveSpeed * deltaTime;
	if (_pos2.y > GWinSizeY + (GWinSizeY / 2))
	{
		_pos2.y -= GWinSizeY * 2.0f;
	}
}

void Map::Render(HDC hdc)
{
	Super::Render(hdc);

	// Map 은 여분으로 동일한 텍스처를 한번더 그려주자.
	if (_textureComponent)
	{
		_textureComponent->RenderComponent(hdc, _pos2);
	}
}
