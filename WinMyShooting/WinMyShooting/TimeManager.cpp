#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void TimeManager::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	// 프레임율(FPS 계산을 위해)
	_frameCount++;
	_frameTime += _deltaTime;

	// 1초당 프레임 카운트가 나온다.
	if (_frameTime >= 1.0f)
	{
		_fps = _frameCount;
		_frameTime -= 1.0f;
		_frameCount = 0;
	}

	// 타이머 관리
	for (auto iter = _timers.begin(); iter != _timers.end();)
	{
		Timer* timer = *iter;
		if (false == timer->Update(_deltaTime))
		{
			// 만료된 타이머
			iter = _timers.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void TimeManager::Destroy()
{
	for (auto iter : _timers)
	{
		SAFE_DELETE(iter);
	}
	_timers.clear();
}

Timer* TimeManager::AddTimer(TimerFunc func, float interval, bool loop)
{
	Timer* timer = new Timer(func, interval, loop);
	_timers.insert(timer);

	return timer;
}

bool Timer::Update(float deltaTime)
{
	_sumTime += deltaTime;
	if (_sumTime >= _interval)
	{
		// 알람이 울려야하는 시간이 되면, 콜백 호출
		_func();

		if (_loop)
		{
			_sumTime -= _interval;
		}
		else
		{
			return false;
		}
	}

	return true;
}
