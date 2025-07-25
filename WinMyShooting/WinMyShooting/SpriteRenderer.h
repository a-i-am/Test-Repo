#pragma once

#include "Component.h"

class Texture;

// 통 텍스처 그리기가 아니라, 하나의 프레임단위로 그리는 객체
class SpriteRenderer : public Component
{
	using Super = Component;

public:
	SpriteRenderer(string textureKey, float dur);

	virtual void InitCompoent() override;
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc, Vector pos) override;

	void SetFrameX(int32 x) { _frameX = x; }
	void SetFrameY(int32 y) { _frameY = y; }

	int32 GetRenderSizeX() { return _renderSizeX; }
	bool IsEnd() { return _isEnd; }
	void SetApplyCameraPos(bool apply) { _applyCameraPos = apply; }

private:
	Texture* _textureInfo = nullptr;	// 리소스 매니저를 통해서 가져온 정보 : 공용 자산

	// 개별 텍스처가 현재 어떤 프레임을 그리고 있는지
	int32 _frameX = 0;
	int32 _frameY = 0;

	// 한 프레임당 사이즈
	int32 _renderSizeX = 0;
	int32 _renderSizeY = 0;

	bool  _isEnd = false;

	float _durtaion = 0;
	float _sumTime = 0;

	bool _applyCameraPos = true;
};

