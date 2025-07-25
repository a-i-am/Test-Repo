#pragma once
#include "UIBase.h"

class Texture;

class UIButton : public UIBase
{
	using Super = UIBase;

public:
	UIButton(Vector pos, string key, int32 width = 0, int32 height = 0);
	virtual ~UIButton();

	void Update() override;
	void Render(HDC renderTarget) override;

	void SetClickEvent(std::function<void()> click) { _onClickedEvent = click; }

private:
	std::function<void()> _onClickedEvent;
	Texture* _texture = nullptr;
};

