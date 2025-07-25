#pragma once

#include "Singleton.h"

// 시간 관리도 프로그램이 살아있는한 유일하게 한개만 존재
// 
// 
// 실습 : 타이머 시스템, 여기다가 코드 추가하시면 되요.
// 타이머가 다 울리면, 콜백 시스템 등록.

class TimeManager : public Singleton<TimeManager>
{
	friend Singleton<TimeManager>;
	
private:
	TimeManager() {}

public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	uint64 _frequency = 0;	// CPU 클럭
	uint64 _prevCount = 0;

	// 이전프레임과 현재 프레임 사이의 시간 가격
	float _deltaTime = 0.f;

private:

	// 초당 몇 프레임인지
	uint32 _frameCount = 0;
	float _frameTime = 0.f;
	uint32 _fps = 0;
};

