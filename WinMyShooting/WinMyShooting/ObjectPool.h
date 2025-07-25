#pragma once
#include <type_traits> 
#include "Actor.h"

// Bullet 정보를 오브젝트 풀로 관리를 하겠다.
// 나중에 Enemy 추가될수도 있다.
// Effect 같은것도 풀로 관리될수 있다.

// -> 범용성을 가지는게 좋을것 같다.
class Actor;

class ObjectPool
{
public:
	template<typename T>
	void Init(int32 size)
	{
		// 초기 설정
		for (int32 i = 0; i < size; ++i)
		{
			// 기본 생성자로 호출할수가 없고.. 객체마다 생성자가 다양하게 존재.
			// 해결하려면.. 기본 구조를 갈아엎어야 한다.
			// 갈아 엎으려고 하니 너무 막막해서.. 
			// 다른 방법으로 new 객체를 만든다. -> 팩토리(패턴)처럼 비슷한 시스템
			T* obj = new T(Vector(0, 0));	
			obj->SetObjectPool(this);

			//_pool.push_back(obj);
			_pool.push(obj);
		}
	}

	void Clear()
	{
		while (!_pool.empty())
		{
			Actor* obj = _pool.top();
			obj->Destroy();
			SAFE_DELETE(obj);

			_pool.pop();
		}
		//for (auto iter : _pool)
		//{
		//	iter->Destroy();
		//	SAFE_DELETE(iter);
		//}
		//_pool.clear();
	}

	// 꺼내쓰기
	template<typename T>
	T* Acquire()
	{
		// 풀에서 꺼내쓰기만 하다보니깐, 풀이 다 비었다.
		if (_pool.empty())
		{
			// 못줘... 줄게 없다. 없으니깐 호출한 함수 니가 알아서 예외처리해라
			// 호출한 함수쪽에서 예외처리하기 너무 힘들다. 무조건 만들어줘.
			//return nullptr;	 

			// 더 만들어서 줄게
			// vector (capacity) 개념
			// vector는 여분으로 조금더 많은 메모리를 할당
			//for(여러개 만들어도)
			// 다쓰고나서 새로 만들어달라고 하는 요청이 빈도가 높다??
			// 필요할때 여러개 만드는걸 고려

			T* obj = new T(Vector(0,0));
			obj->SetObjectPool(this);

			//T* obj = factory(); // 함수포인터로 new 객체를 만들겠다. 왜냐면 현재 구조상 생성자가 너무 다양해서..
			//_pool.push_back(obj);
			_pool.push(obj);
		}

		// 벡터니깐 마지막에서 꺼내쓰는게 효율이 좋다.
		//T* obj = static_cast<T*>(_pool.back());
		//_pool.pop_back();

		T* obj = static_cast<T*>(_pool.top());
		_pool.pop();

		return obj;
	}

	// 반환하기
	void Return(Actor* obj)
	{
		if (nullptr == obj)
			return;

		_pool.push(obj);
		//_pool.push_back(obj);
	}

public:
	//vector<Actor*> _pool; // 어떤 자료형이 될지 모르겟지만, 여러개를 한번에 생성
	stack<Actor*> _pool;
};

