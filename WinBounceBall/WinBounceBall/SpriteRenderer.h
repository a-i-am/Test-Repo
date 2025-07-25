#pragma once
#include "Component.h"


class SpriteRenderer : public Component
{
	using Super = Component;

public:
	SpriteRenderer(string key, int32 width, int32 height);
	virtual ~SpriteRenderer();

	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC renderTarget, Vector pos) override;

	void SetSpriteDuration(float duration) { _durtaion = duration; }
	void SetLoop(bool loop) { _loop = loop; }
	void SetSpriteIndex(int32 x, int32 y) { _animIndexX = x; _animIndexY = y; }
	
	Size GetFrameSize();
	bool GetLoop() { return _loop; }
	bool IsEnd() { return _isEnd; }
	void GetIndex(int32& outX, int32& outY) { outX = _animIndexX; outY = _animIndexY; }
	int32 GetMaxFrameCount();
	string GetSpriteKey() { return _spriteKey; }


protected:
	class Texture* _texture = nullptr;
	string _spriteKey;

	int32 _animIndexX = 0;
	int32 _animIndexY = 0;
	bool _isEnd = false;
	bool _loop = false;
	float _durtaion = 0.2f;
	float _sumTime = 0;
};