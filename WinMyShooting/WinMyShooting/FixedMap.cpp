#include "pch.h"
#include "FixedMap.h"
#include "ImageRenderer.h"
#include "Texture.h"

FixedMap::FixedMap(Vector pos) : Super(pos)
{
	_textureComponent = CreateImageComponent("BG_2");
}

void FixedMap::Init()
{
	Super::Init();
}

void FixedMap::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void FixedMap::Render(HDC hdc)
{
	Super::Render(hdc);
}

int32 FixedMap::GetTextureWidth()
{
	if (_textureComponent)
	{
		return _textureComponent->GetTexture()->_sizeX;
	}
	return GWinSizeX;
}

int32 FixedMap::GetTextureHeight()
{
	if (_textureComponent)
	{
		return _textureComponent->GetTexture()->_sizeY;
	}
	return GWinSizeY;
}