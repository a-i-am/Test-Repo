#include "pch.h"
#include "ImageRenderer.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Texture.h"

ImageRenderer::ImageRenderer(string key, int32 width, int32 height)
{
	_texture = ResourceManager::GetInstance()->GetTexture(key);
	if (_texture == nullptr)
	{
		::MessageBoxA(Game::GetInstance()->GetHwnd(), key.c_str(), "Invalid Texture Load", MB_OK);
		return;
	}

	if (width != 0 || height != 0)
	{
		_texture->SetSize(width, height);
	}
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::RenderComponent(HDC renderTarget, Vector pos)
{
	if (_texture)
	{
		_texture->Render(renderTarget, pos, Vector(0, 0));
	}
}

Size ImageRenderer::GetSize()
{
	if (_texture)
	{
		return Size(_texture->GetSizeX(), _texture->GetSizeY());
	}

	return Size(0, 0);
}

void ImageRenderer::SetCenterAlign(bool center)
{
	if (_texture)
	{
		_texture->SetCenterAlign(center);
	}
}
