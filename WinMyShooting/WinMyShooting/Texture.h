#pragma once

// 텍스처를 그리는 역할
// 한곳에서 렌더링을 담당하기 위해 리팩토링
class Texture
{
public:
	Texture();

	void Load(string key, string texturePath, int32 transparent, int32 xFrameCount = 1, int32 yFrameCount = 1);
	void Render(HDC hdc, Vector pos, Vector src, Vector rederSize, bool applyCameraPos = true);

	int32 GetTextureWidth()
	{
		return _sizeX;
	}

public:
	// 텍스처를 그리기위한 변수
	HDC			_textureHdc = 0;
	HBITMAP		_bitmap;
	int32		_transparent = -1;	// 투명하게 보여야하는 색상
	int32		_sizeX = 0;
	int32		_sizeY = 0;
	int32		_frameCountX = 0;	// X축으로 몇개의 프레임이 있는지
	int32		_frameCountY = 0;	// Y축으로 몇개의 프레임이 있는지
};

