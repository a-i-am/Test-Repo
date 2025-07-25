#pragma once

#include "Singleton.h"

// 시간 관리도 프로그램이 살아있는한 유일하게 한개만 존재
// 
// 
// 실습 : 타이머 시스템, 여기다가 코드 추가하시면 되요.
// 타이머가 다 울리면, 콜백 시스템 등록.

using TimerFunc = std::function<void()>;	// 함수포인터 or std::function

class Timer
{
public:
	Timer(TimerFunc func, float interval, bool loop) : _func(func), _interval(interval), _loop(loop) {}

	// 반환값 : 타이머가 만료되었는지 (true : 유지, false:만료)
	bool Update(float deltaTime);

private:
	//@TODO: 콜백 받는 객체의 생명 주기도 신경써야 한다.
	// 만약, 타이머가 사라지게 되면, 콜백받는 객체한테 알려야 할수도 있다.
	// 콜백 시스템을 사용하게 되면, 늘 생명주기에 대해서 신경을 쓰자.
	// 그게 싫으면 스마트 포인터 사용하는것도 답이 된다.
	TimerFunc	_func;		


	float		_sumTime = 0;
	float		_interval = 0;
	bool		_loop = false;
};

class TimeManager : public Singleton<TimeManager>
{
	friend Singleton<TimeManager>;
	
private:
	TimeManager() {}

public:
	void Init();
	void Update();
	void Destroy();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

	// 타이머를 등록해놓고, 등록한 객체 자체가 사라질수도 있으니깐, 예외처리를 해야한다.
	Timer* AddTimer(TimerFunc func, float interval, bool loop);

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

	// 타이머 관리
	unordered_set<Timer*> _timers;
};

