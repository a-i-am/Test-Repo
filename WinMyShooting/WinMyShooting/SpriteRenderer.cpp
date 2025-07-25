#include "pch.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"

SpriteRenderer::SpriteRenderer(string textureKey, float dur)
{
	_textureInfo = ResourceManager::GetInstance()->GetTextureInfo(textureKey);
	_durtaion = dur;

	// 한 프레임의 사이즈를 미리 계산
	//@TODO : TextureInfo 에 추가하는걸 고려 : 공용 자산. 객체마다 다르게 적용할일이 없기
	if (_textureInfo)
	{
		int32 renderSizeX = _textureInfo->_frameCountX != 0 ? (_textureInfo->_sizeX / _textureInfo->_frameCountX) : _textureInfo->_sizeX;
		int32 renderSizeY = _textureInfo->_frameCountY != 0 ? (_textureInfo->_sizeY / _textureInfo->_frameCountY) : _textureInfo->_sizeY;
		
		_renderSizeX = renderSizeX;
		_renderSizeY = renderSizeY;
	}
}

void SpriteRenderer::InitCompoent()
{
	Super::InitCompoent();
}

void SpriteRenderer::UpdateComponent(float deltaTime)
{
	Super::UpdateComponent(deltaTime);

	if (_durtaion == 0)
		return;

	if (_isEnd)
		return;

	if (_textureInfo == nullptr)
		return;

	// 본인의 프레임 계산을 하는 로직
	_sumTime += deltaTime;

	int32 totalCount = _textureInfo->_frameCountX * _textureInfo->_frameCountY;
	float delta = _durtaion / totalCount;

	// 일정 시간이 지나면 다음 프레임 이동
	if (_sumTime >= delta && _isEnd == false)
	{
		if (_frameX + 1 >= _textureInfo->_frameCountX)
		{
			_frameX = 0;

			if (_frameY + 1 >= _textureInfo->_frameCountY)
			{
				_frameY = 0;

				//if (bitmapInfo->loop == false)
				{
					_isEnd = true;
				}
			}
			else
			{
				++_frameY;
			}
		}
		else
		{
			++_frameX;
		}

		_sumTime -= delta;
	}
}

void SpriteRenderer::RenderComponent(HDC hdc, Vector pos)
{
	Super::RenderComponent(hdc, pos);

	if (_textureInfo == nullptr)
		return;

	// 원본텍스처의 시작 위치를 계산한다.
	// 텍스처의 특정 frame 으로 그려야 해서
	int32 srcX = _frameX * _renderSizeX;
	int32 srcY = _frameY * _renderSizeY;

	_textureInfo->Render(hdc, pos, Vector(srcX, srcY), Vector(_renderSizeX, _renderSizeY), _applyCameraPos);
}
