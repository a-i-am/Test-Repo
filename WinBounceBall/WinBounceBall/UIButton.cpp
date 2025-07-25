#include "pch.h"
#include "UIButton.h"
#include "InputManager.h"
#include "Texture.h"
#include "ResourceManager.h"

UIButton::UIButton(Vector pos, string key, int32 width, int32 height) : Super(pos)
{
	_texture = ResourceManager::GetInstance()->GetTexture(key);
	if (width != 0 || height != 0)
	{
		_texture->SetSize(width, height);
	}
	_texture->SetCenterAlign(false);
}

UIButton::~UIButton()
{
}

void UIButton::Update()
{
	if (_texture == nullptr)
		return;

	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		RECT rect;
		rect.left = (int32)_pos.x;
		rect.right = (int32)_pos.x + _texture->GetSizeX();
		rect.top = (int32)_pos.y;
		rect.bottom = (int32)_pos.y + _texture->GetSizeY();

		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		if (IsInPoint(rect, mousePos))
		{
			if (_onClickedEvent)
				_onClickedEvent();
		}
	}
}

void UIButton::Render(HDC renderTarget)
{
	if (_texture)
	{
		_texture->Render(renderTarget, _pos, Vector(0, 0));
	}
}
