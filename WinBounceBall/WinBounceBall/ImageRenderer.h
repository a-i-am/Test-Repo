#pragma once

#include "Component.h"


class ImageRenderer : public Component
{
	using Super = Component;
public:
	ImageRenderer(string key, int32 width, int32 height);
	virtual ~ImageRenderer();

	void RenderComponent(HDC renderTarget, Vector pos) override;
	Size GetSize();
	void SetCenterAlign(bool center);

private:
	class Texture* _texture = nullptr;
};
