#pragma once
#include "Scene.h"

class EditScene : public Scene
{
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltatTime) override;
	virtual void Render(HDC hdc) override;

private:
	void save_basic(std::wstring fileName);
	void load_basic(std::wstring fileName);

	void save_dialog();
	void load_dialog();

private:
	vector<pair<POINT, POINT>> _lines;	// 일종의 리소스를 만들수 있다.

	bool _drawing = false;
	POINT _startPos;
};

