#pragma once

// 싱글톤을 만들고 싶다면 상속받아서 사용
template<typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		// 1) 힙 할당
		//if (_instance == nullptr)
		//{
		//	_instance = new T();
		//}
		//return _instance;

		// 2) static 변수 : 안정성
		return &_instance;
	}
	static void DestroyInstance()
	{
		//if (_instance != nullptr)
		//{
		//	delete _instance;
		//	_instance = nullptr;
		//}
	}

protected:
	// 생성자와 소멸자를 protected로 설정하여 외부에서 직접 생성/삭제를 방지
	Singleton() = default;
	virtual ~Singleton() = default;

	// 복사 및 이동 금지
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

private:
	//static T* _instance;	// 이 버전으로도 상속구조로 싱글턴 생성 가능
	static T _instance;	// 이 버전으로도 상속구조로 싱글턴 생성 가능
};

// 정적 멤버 초기화
//template<typename T>
//T* Singleton<T>::_instance = nullptr;

// 정적 멤버 정의
template<typename T>
T Singleton<T>::_instance;