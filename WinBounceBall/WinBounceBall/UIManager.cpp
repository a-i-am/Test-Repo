#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "UIBase.h"
#include "UIButton.h"
#include "UIImage.h"


void UIManager::Destroy()
{
	DestroyAllWidget();
}

void UIManager::Init()
{

}

void UIManager::Update()
{
	for (auto iter : _uiWidget)
	{
		if (iter->IsOpen())
		{
			iter->Update();
		}
	}
}

void UIManager::Render(HDC hdc)
{
	for (auto iter : _uiWidget)
	{
		if (iter->IsOpen())
		{
			iter->Render(hdc);
		}
	}
}

void UIManager::DestroyAllWidget()
{
	for (auto iter : _uiWidget)
	{
		SAFE_DELETE(iter);
	}
	_uiWidget.clear();
}

UIButton* UIManager::CreateButton(Vector pos, string key, int32 width, int32 height)
{
	UIButton* button = new UIButton(pos, key);
	_uiWidget.emplace_back(button);
	return button;
}

UIImage* UIManager::CreateImage(Vector pos, string key, int32 width, int32 height)
{
	UIImage* image = new UIImage(pos, key);
	_uiWidget.emplace_back(image);
	return image;
}




