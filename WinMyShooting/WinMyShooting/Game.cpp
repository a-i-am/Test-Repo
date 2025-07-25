#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "DataManager.h"

#include "GameScene.h"

void Game::Init(HWND hwnd)
{
	srand((uint32)time(0));

	_hwnd = hwnd;
	_hdc = ::GetDC(hwnd);	// 기본 도화지 넘겨받기

	// 더블 버퍼링 버퍼 추가
	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // 기본 hdc와 호환되는 DC를 생성

	// 이게 추가로 필요한 이유는 생성된 호환 hdc는 우리가 원하는 크기의 버퍼가 아니라 아주 작은
	// 버퍼이다. 우리는 우리 게임의 전체 정보를 그릴 도화지가 필요한거라
	// 별도의 비트맵을 만들어서, 여기다가 정보를 그릴것이다.
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성

	// 여기다가 생성된 백버퍼 HDC와 게임사이즈에 맞는 텍스쳐를 연결한다.
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
	::DeleteObject(prev);


	// 게임과 관련된 초기화가 필요한 항목들은 여기서
	InputManager::GetInstance()->Init(hwnd);	// 객체를 생성
	TimeManager::GetInstance()->Init();
	CollisionManager::GetInstance()->Init();

	DataManager::GetInstance()->Init();
	// 모~든 리소스를 한번에 다 로딩하는게 맞을까?
	// 로비씬 / 게임씬 -> 리소스를 다르게 로딩하게 수정한다.
	ResourceManager::GetInstance()->Init();	// 모든 리소스 로딩

	// 시작씬은 게임으로 설정
	_currScene = new GameScene();
	_currScene->Init();
}

void Game::Destroy()
{
	// 게임과 관련된 삭제가 필요한 항목들은 여기서
	InputManager::DestroyInstance();	// 객체를 생성

	TimeManager::GetInstance()->Destroy();
	TimeManager::DestroyInstance();
	
	CollisionManager::GetInstance()->DestroyInstance();
	DataManager::GetInstance()->Destory();
	ResourceManager::GetInstance()->Destroy();

	if (_currScene)
	{
		_currScene->Destroy();
	}
	SAFE_DELETE(_currScene);
}

void Game::Update()
{
	InputManager::GetInstance()->Update();
	TimeManager::GetInstance()->Update();

	if (_currScene)
	{
		_currScene->Update(TimeManager::GetInstance()->GetDeltaTime());
		_currScene->PostUpdate(TimeManager::GetInstance()->GetDeltaTime());

		CollisionManager::GetInstance()->Update(TimeManager::GetInstance()->GetDeltaTime());
	}
}

void Game::Render()
{
	// 여기서는 프론트 버퍼에 그림을 그리지 않고, 백버퍼에 그림을 그린다.
	if (_currScene)
	{
		_currScene->Render(_hdcBack);
		CollisionManager::GetInstance()->Render(_hdcBack);
	}

	// 현재 프레임률 정보를 찍어보자.
	HFONT font = ResourceManager::GetInstance()->GetFont(FontSize::Font26);
	SelectObject(_hdcBack, font);

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetInstance()->GetDeltaTime();
	SetTextColor(_hdcBack, RGB(255, 255, 255)); // 흰색
	SetBkMode(_hdcBack, TRANSPARENT);           // 배경 투명
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 300, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int32>(str.size()));
	}


	// 여분 도화지에 렌더링 끝.
	// 한번 그림을 다 그렸으니, 이제는 프론트 버퍼에 복사.
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사

	// 프론트 버퍼에 복사가 끝났으면, 백버퍼는 초기화
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
