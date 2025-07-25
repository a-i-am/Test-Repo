#pragma once

enum class TextureType
{
	BMP,
	PNG
};


class Texture
{
public:
	void Load(wstring fileName, int32 maxCountX, int32 maxCountY, int32 transparent);
	void Destroy();

	// 해당 리소스가 그려지는 부분
	void Render(HDC hdc, Vector pos, Vector srcPos);

	void SetCenterAlign(bool center) { _centerAlign = center; }
	void SetSize(int32 x, int32 y) { _sizeX = x; _sizeY = y; }

	int32 GetSizeX() { return _sizeX; }
	int32 GetSizeY() { return _sizeY; }

	Size GetFrameSize() { return Size(_frameSizeX, _frameSizeY); }
	void GetFrameCount(int32& outX, int32& outY) { outX = _maxCountX; outY = _maxCountY; }

private:
	// 텍스처를 그리기위한 변수
	HDC		_textureHdc = 0;
	HBITMAP _bitmap;

	// GDI+ (png)
	Gdiplus::Image* _img = nullptr;

	TextureType _type;
	int32 _transparent = -1;	// 투명하게 보여야하는 색상
	int32 _textureSizeX = 0;	// 원본 텍스처 크기
	int32 _textureSizeY = 0;

	int32 _sizeX = 0;	// 텍스처 크기와는 상관없이 원하는 크기
	int32 _sizeY = 0;

	int32	_maxCountX = 0;
	int32	_maxCountY = 0;
	int32	_frameSizeX = 0;	// Sprite 처럼 한장의 이미지에 합쳐있다면, 한칸의 프레임 사이즈
	int32	_frameSizeY = 0;

	// 기본은 가운데 위치를 기준으로 텍스처 그리기
	bool _centerAlign = true;
};

