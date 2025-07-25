#include "pch.h"
#include "Texture.h"
#include "Game.h"

void Texture::Load(wstring fileName, int32 maxCountX, int32 maxCountY, int32 transparent)
{
	fs::path fullPath = fs::current_path() / L"../Resources/Image/" / fileName;

	if (fileName.find(L".png") != std::wstring::npos)
	{
		// GDI 로드
		_type = TextureType::PNG;
		_img = Gdiplus::Image::FromFile(fullPath.c_str());
		if (_img == nullptr)
		{
			return;
		}
		_textureSizeX = _img->GetWidth();
		_textureSizeY = _img->GetHeight();
	}
	else
	{
		// WinAPI 텍스처 로딩하는 방법
		_type = TextureType::BMP;
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

		_textureSizeX = bit.bmWidth;
		_textureSizeY = bit.bmHeight;
	}

	_sizeX = _textureSizeX;
	_sizeY = _textureSizeY;

	_maxCountX = maxCountX;
	_maxCountY = maxCountY;
	_frameSizeX = _textureSizeX / _maxCountX;
	_frameSizeY = _textureSizeY / _maxCountY;
}

void Texture::Destroy()
{
}

void Texture::Render(HDC hdc, Vector pos, Vector srcPos)
{
	Vector renderPos = _centerAlign ? Vector(pos.x - _sizeX * 0.5f, pos.y - _sizeY * 0.5f) : pos;

	// GDI+
	if (_type == TextureType::PNG)
	{
		::Graphics g(hdc);
		g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		g.DrawImage(_img, renderPos.x, renderPos.y);
	}
	else
	{

		// Bitmap
		if (_transparent == -1)
		{
			::BitBlt(hdc,	// 백버퍼에
				(int32)renderPos.x,	// 텍스처를 중심좌표로 그리기위해 size의 절반만큼 빼준다.
				(int32)renderPos.y,
				_sizeX,
				_sizeY,
				_textureHdc,	// 텍스쳐 그리기
				(int32)srcPos.x,
				(int32)srcPos.y,
				SRCCOPY);
		}
		else
		{
			::TransparentBlt(hdc,
				(int32)renderPos.x,	// 텍스쳐가 그려져야하는 좌표 X			// pos.x
				(int32)renderPos.y,	// 텍스쳐가 그려져야하는 좌표 Y			// pos.y
				_sizeX,							// 텍스쳐가 그려져야하는 크기 width		// 64
				_sizeY,							// 텍스쳐가 그려져야하는 크기 height	// 64
				_textureHdc,					// 텍스처의 정보
				(int32)srcPos.x,								// 원본 텍스쳐의 X						// 0~15번의 인덱스로 돌아가면서 그려야한다.
				(int32)srcPos.y,								// 원본 텍스쳐의 Y	
				_frameSizeX,							// 원본 텍스쳐의 width					// 64
				_frameSizeY,							// 원본 텍스쳐의 height					// 64
				_transparent);	// 어떤색상을 투명하게 그릴까
		}
	}
}
