#pragma once

#include "Component.h"
class Texture;

class ImageRenderer : public Component
{
	using Super = Component;
public:
	ImageRenderer(string textureKey);

	virtual void InitCompoent() override;
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc, Vector pos) override;

	int32 GetTextureWidth();
	Texture* GetTexture() { return _textureInfo; }
	void SetApplyCameraPos(bool apply) { _applyCameraPos = apply; }

private:
	Texture* _textureInfo = nullptr;	// 리소스 매니저를 통해서 가져온 정보 : 공용 자산
	bool _applyCameraPos = true;
};


