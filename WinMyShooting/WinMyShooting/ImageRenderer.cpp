#include "pch.h"
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"

ImageRenderer::ImageRenderer(string textureKey)
{
	_textureInfo = ResourceManager::GetInstance()->GetTextureInfo(textureKey);
}

void ImageRenderer::InitCompoent()
{
	Super::InitCompoent();
}

void ImageRenderer::UpdateComponent(float deltaTime)
{
	Super::UpdateComponent(deltaTime);
}

void ImageRenderer::RenderComponent(HDC hdc, Vector pos)
{
	Super::RenderComponent(hdc, pos);

	if (_textureInfo == nullptr)
		return;

	_textureInfo->Render(hdc, pos, Vector(0, 0), Vector(_textureInfo->_sizeX, _textureInfo->_sizeY), _applyCameraPos);
}

int32 ImageRenderer::GetTextureWidth()
{
	return _textureInfo->GetTextureWidth();
}