#include "pch.h"
#include "Texture.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Camera.h"

Texture::Texture()
{
}

void Texture::Load(string key, string texturePath, int32 transparent, int32 xFrameCount, int32 yFrameCount)
{
	fs::path fullPath = fs::current_path() / L"../Resources/" / texturePath;

	HDC hdc = ::GetDC(Game::GetInstance()->GetHwnd());

	_textureHdc = ::CreateCompatibleDC(hdc);
	_bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		fullPath.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (_bitmap == 0)
	{
		::MessageBox(Game::GetInstance()->GetHwnd(), fullPath.c_str(), L"Invalid Texture Load", MB_OK);
		return;
	}

	_transparent = transparent; //RGB(252, 0, 255);

	HBITMAP prev = (HBITMAP)::SelectObject(_textureHdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);

	_sizeX = bit.bmWidth;
	_sizeY = bit.bmHeight;

	_frameCountX = xFrameCount;
	_frameCountY = yFrameCount;
}

void Texture::Render(HDC hdc, Vector pos, Vector src, Vector rederSize, bool applyCameraPos)
{
	// 그림을 그리는데, pos 위치에 그림을 그린다.
	// pos 월드 좌표계(맵 기준의 절대 좌표)
	// 카메라 기준으로 좌표 변환을 해준다.

	Vector screenPos = applyCameraPos ? Camera::ConvertScreenPos(pos) : pos;


	if (_transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)screenPos.x - (rederSize.x / 2),	// 텍스처를 중심좌표로 그리기위해 size의 절반만큼 빼준다.
			(int32)screenPos.y - (rederSize.y / 2),
			rederSize.x,
			rederSize.y,
			_textureHdc,	// 텍스쳐 그리기
			src.x,
			src.y,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)screenPos.x - (rederSize.x / 2),	// 텍스처를 중심좌표로 그리기위해 size의 절반만큼 빼준다.
			(int32)screenPos.y - (rederSize.y / 2),
			rederSize.x,							// 텍스쳐가 그려져야하는 크기 width		// 64
			rederSize.y,							// 텍스쳐가 그려져야하는 크기 height	// 64
			_textureHdc,					// 텍스처의 정보
			src.x,								// 원본 텍스쳐의 X						// 0~15번의 인덱스로 돌아가면서 그려야한다.
			src.y,								// 원본 텍스쳐의 Y	
			rederSize.x,							// 원본 텍스쳐의 width					// 64
			rederSize.y,							// 원본 텍스쳐의 height					// 64
			_transparent);	// 어떤색상을 투명하게 그릴까
	}

	//if (transparent == -1)
	//{
	//	::BitBlt(hdc,	// 백버퍼에
	//		(int32)pos.x - (sizeX / 2),	// 텍스처를 중심좌표로 그리기위해 size의 절반만큼 빼준다.
	//		(int32)pos.y - (sizeY / 2),
	//		sizeX,
	//		sizeY,
	//		textureHdc,	// 텍스쳐 그리기
	//		0,
	//		0,
	//		SRCCOPY);
	//}
	//else
	//{
	//	::TransparentBlt(hdc,
	//		(int32)pos.x - (sizeX / 2),	// 텍스쳐가 그려져야하는 좌표 X			// pos.x
	//		(int32)pos.y - (sizeY / 2),	// 텍스쳐가 그려져야하는 좌표 Y			// pos.y
	//		sizeX,							// 텍스쳐가 그려져야하는 크기 width		// 64
	//		sizeY,							// 텍스쳐가 그려져야하는 크기 height	// 64
	//		textureHdc,					// 텍스처의 정보
	//		0,								// 원본 텍스쳐의 X						// 0~15번의 인덱스로 돌아가면서 그려야한다.
	//		0,								// 원본 텍스쳐의 Y	
	//		sizeX,							// 원본 텍스쳐의 width					// 64
	//		sizeY,							// 원본 텍스쳐의 height					// 64
	//		transparent);	// 어떤색상을 투명하게 그릴까
	//}
}