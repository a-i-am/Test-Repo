#pragma once

#include "Singleton.h"

enum class KeyState
{
	None,
	Press,		// 누르고 있는 중
	Down,		// 방금 누름
	Up,			// 방금 뗏음

	End
};

// 본인만의 프로젝트에서 필요한 키보드 값을 매핑을 해준다.
enum class KeyType
{
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,
	SpaceBar = VK_SPACE,

	// 아이템 사용, 1,2
	KEY_1 = '1',
	KEY_2 = '2',

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	L = 'L',
	Q = 'Q',
	E = 'E',
	T = 'T',

	F1 = VK_F1,
	F2 = VK_F2,
	F3 = VK_F3,

	F7 = VK_F7,

};

// 언리얼의 방식
constexpr int32 KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX) + 1;

// window api 함수를 래핑해서 우리만의 편한 InputSystem 을 만들자.
class InputManager : public Singleton<InputManager>
{
	friend Singleton<InputManager>;

private:
	InputManager() {}

public:

	void Init(HWND hwnd);
	void Update();

	// 키보드 입력 처리 : 키보드가 방금 눌러졌나? 방금 떨어졌나? 프레스 중인가?
	// 누르고 있을 때
	bool GetButtonPressed(KeyType key) { return GetState(key) == KeyState::Press; }

	// 맨 처음 눌렀을 때
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }

	// 맨 처음 눌렀다가 땔 때
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	// 마우스 좌표 얻어오기
	POINT GetMousePos() { return _mousePos; }

	void SetEventMouseWheel(std::function<void(int32)> event) { _onMouseWheel = event; }
	void OnEventMouseWheel(int32 delta) { if (_onMouseWheel) _onMouseWheel(delta); }

private:
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd = 0;	// 윈도우 핸들
	vector<KeyState> _states;
	POINT _mousePos = {};

	std::function<void(int32)> _onMouseWheel;
};

