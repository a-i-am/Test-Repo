#include "pch.h"
#include "UIImage.h"
#include "Texture.h"
#include "ResourceManager.h"

UIImage::UIImage(Vector pos, string key, int32 width, int32 height) : Super(pos)
{
	_texture = ResourceManager::GetInstance()->GetTexture(key);
	if (width != 0 || height != 0)
	{
		_texture->SetSize(width, height);
	}
	_texture->SetCenterAlign(false);
}

UIImage::~UIImage()
{
}

void UIImage::Update()
{
}

void UIImage::Render(HDC renderTarget)
{
	if (_texture)
	{
		_texture->Render(renderTarget, _pos, Vector(0, 0));
	}
}
