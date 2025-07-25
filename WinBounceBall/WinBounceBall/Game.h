#pragma once

#include "Singleton.h"
// 싱글톤 패턴
// 특징1) 단 하나만 존재
// 특징2) 어떤 코드에서도 바로바로 객체를 얻어올수 있다.

// 전체 게임 관리하는 매니저
class Game : public Singleton<Game>
{
	// 그닥 쓸일이 많지않아서 제가 소개는 안한 C++ 키워드 문법
	friend Singleton<Game>;		// Singleton<Game> 클래스는 내 친구라고 선언

protected:
	Game() {}
	
public:

	// 게임이 현재, 메뉴 모드 / 캐릭터 선택창 / 편집 모드/ 게임 실행

	void Init(HWND	hwnd);	// 초기화
	void Destroy();	// 삭제

	void Update();	// 업데이트
	void Render();	// 렌더링

	HWND GetHwnd() { return _hwnd; }
	class Scene* GetCurrScene() { return _currScene; }

public:
	void ChangeLobbyScene();
	void ChangeGameScene();
	void ChangeEditorScene();

private:
	HWND	_hwnd;	// 윈도우 핸들
	HDC		_hdc;	// 기본 도화지 정보 (프론트 버퍼)

	// Double Buffering
	RECT	_rect;              // 버퍼의 영역
	HDC		_hdcBack = {};	// 여분의 도화지 준비
	HBITMAP _bmpBack = {};	// Bitmap 에다가 써야한다.

	// GDI+
	ULONG_PTR _gdiplusToken;
	GdiplusStartupInput  _gdiplusStartupInput;

	class Scene* _currScene = nullptr;	// 현재 활성화된 씬만 업데이트/그려준다.
	class Scene* _nextScene = nullptr;	// 현재 씬이 모두 끝나고 다음 틱에 변경
};

