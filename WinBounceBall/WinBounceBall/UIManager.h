#pragma once

//#include "Singleton.h"

class UIBase;
class UIButton;
class UIImage;

class UIManager
{
public:
	void Init();
	void Destroy();

	void Update();
	void Render(HDC hdc);

	void DestroyAllWidget();

	UIButton* CreateButton(Vector pos, string key, int32 width = 0, int32 height = 0);
	UIImage* CreateImage(Vector pos, string key, int32 width = 0, int32 height = 0);

private:
	vector<UIBase*> _uiWidget;
};

