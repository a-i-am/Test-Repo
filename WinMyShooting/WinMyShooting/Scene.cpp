#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "CollisionManager.h"
#include "ObjectPool.h"
#include "Camera.h"

bool Scene::drawDebugCell = false;

void Scene::Init()
{
	// 필요한 리소스 로드
	loadResource();

	// 뒷배경 맵을 먼저 생성
	createBackgroundObject();

	// 그리드 초기화
	_gridCountX = GetMapSizeX() / GRID_SIZE;
	_gridCountY = GetMapSizeY() / GRID_SIZE;

	for (int32 i = 0; i < _gridCountX; ++i)
	{
		for (int32 j = 0; j < _gridCountY; ++j)
		{
			Cell cell{ i, j };	
			CellInfo data;
			_grid.insert(std::make_pair(cell, data));
		}
	}

	// 시작 오브젝트 생성
	createDefaultObject();
}

void Scene::Destroy()
{
	_reserveAdd.clear();
	_reserveRemove.clear();

	for (auto layer : _renderList)
	{
		layer.clear();
	}

	for (auto iter : _actors)
	{
		iter->Destroy();
		SAFE_DELETE(iter);
	}
	_actors.clear();
}

void Scene::Update(float deltaTime)
{
	// 모든 액터의 업데이트가 끝난 이후, Post Update에서 처리
	for (auto actor : _actors)
	{
		// 1. Player
		// 2. Bullet -> 화면밖으로 나갓다. delete
		// 3. Enemy -> 충돌체크할때, Bullet의 좌표가 밖이기, 
		actor->Update(deltaTime);

		// 예약 시스템으로 삭제시 장점
		// Update 로직에서는 무조건 객체가 살아있어요.
	}
}

// Update 가 다 끝난 직후 예약된 액터를 추가한다.
void Scene::PostUpdate(float deltaTime)
{
	// 예약된 객체를 추가
	for (auto actor : _reserveAdd)
	{
		addActor(actor);
	}
	_reserveAdd.clear();


	// 예약된 객체를 삭제, A Enemy 2번
	for (auto actor : _reserveRemove)
	{
		removeActor(actor);	// 진짜 메모리를 해제
	}
	_reserveRemove.clear();
}

void Scene::Render(HDC hdc)
{
	// 이렇게 일일히 순서를 지정하지 않고, Layer 개념으로 한다
	// _player.Render();
	// _effect.Render();

	// 렌더링 순서를 명시적으로 지정하기 위해서 별도의 컨테이너로 관리
	//for (auto actor : _actors)
	for(auto layer : _renderList)
	{
		for (auto actor : layer)
		{
			actor->Render(hdc);
		}
	}

	// 그리드 디버그용
	if (drawDebugCell)
	{
		drawGrid(hdc);
	}
}

// 빨간색 그리드 배경
void Scene::drawGrid(HDC hdc)
{
	// 화면 크기와 그리드 크기 설정
	int32 width = _mapSizeX;
	int32 height = _mapSizeY;

	// 빨간색 펜 생성
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

	// 가로선 그리기
	for (int y = 0; y <= height; y += GRID_SIZE)
	{
		Vector start = Camera::ConvertScreenPos(Vector(0, y));
		Vector end = Camera::ConvertScreenPos(Vector(width, y));

		MoveToEx(hdc, start.x, start.y, nullptr); // 시작점 설정
		LineTo(hdc, end.x, end.y);        // 끝점까지 선 그리기
	}

	// 세로선 그리기
	for (int x = 0; x <= width; x += GRID_SIZE)
	{
		Vector start = Camera::ConvertScreenPos(Vector(x, 0));
		Vector end = Camera::ConvertScreenPos(Vector(x, height));

		MoveToEx(hdc, start.x, start.y, nullptr); // 시작점 설정
		LineTo(hdc, end.x, end.y);        // 끝점까지 선 그리기
	}

	// 이전 펜 복원 및 새 펜 삭제
	SelectObject(hdc, oldPen);
	DeleteObject(redPen);
}


void Scene::ReserveRemove(Actor* actor)
{
	// 적,
	// 나의 총알
	// 적 근처에 나의 총알 2개 겹쳐있다.
	//auto findIter = std::find(_reserveRemove.begin(), _reserveRemove.end(), actor);
	// 1번째 총알 부딪히면 Enemy (A) : Dead
	// 2번째 총알 부딪히면 Enemy (A) : Dead는 무시
	for (auto iter : _reserveRemove)
	{
		if (iter == actor)
			return;
	}

	_reserveRemove.push_back(actor);
}

void Scene::UpdateGrid(Actor* actor, Vector prevPos, Vector newPos)
{
	// 이전 Cell 위치와 현재 Cell 위치를 비교해서 
	// Cell Info 값을 갱신한다.
	Cell prevCell = Cell::ConvertToCell(prevPos, GRID_SIZE);
	Cell currCell = Cell::ConvertToCell(newPos, GRID_SIZE);

	// 같으니 갱신 필요 없음
	if (prevCell == currCell)
		return;

	// 셀 갱신을 시작
	// 
	// 이전 셀에서는 삭제
	{
		auto find = _grid.find(prevCell);

		// 좌표가 유효하지 않아서, Cell 정보가 이상하게 넘어와도 _grid에 있는것만 처리
		// 좌표가 0,0 보다 작아지면 -1,-1
		// 좌표가 maxX , maxY 보다 커지면, 그리의 인덱스가 초기에 만들었던 것보다 크게 넘어온다.
		// Grid : 10, 10 -> 11, 11
		if (find != _grid.end())	
		{
			// 복사복이 아니라 원본을 수정할거라서 Refrence auto 만들었다
			auto& cellInfo = find->second;	// Cell Info

			// 셀이 관리하고 있는 N개의 액터중에 진짜로 현재 Actor가 있는지 검사
			auto findActor = cellInfo.actorList.find(actor);
			if (findActor != cellInfo.actorList.end())
			{
				cellInfo.actorList.erase(findActor);
			}
			else
			{
				// 예외 발생. 내가 관리하는 그리드에 문제가 생겼다.
				// 로그 출력.
				//OutputDebugString(L"Invalid Grid!!");
			}
		}
	}

	// 현재 추가하려는 셀이 범위 밖이면 무시
	if (currCell.index_X < 0 || currCell.index_X >= _gridCountX || currCell.index_Y < 0 || currCell.index_Y >= _gridCountY)
		return;

	// 현재 셀에는 추가
	{
		auto find = _grid.find(currCell);
		if (find != _grid.end())
		{
			// 복사복이 아니라 원본을 수정할거라서 Refrence auto 만들었다
			auto& cellInfo = find->second;	// Cell Info

			// 셀이 관리하고 있는 N개의 액터중에 진짜로 현재 Actor가 있는지 검사
			auto findActor = cellInfo.actorList.find(actor);
			if (findActor != cellInfo.actorList.end())
			{
				// 새로운 Cell 정보인데, 이미 그 셀에 해당 Actor가 추가가 되었다면
				// 그리가 꼬인것.
				// 로그 출력.
				//OutputDebugString(L"Invalid Grid!!");
			}
			else
			{
				cellInfo.actorList.insert(actor);
			}
		}
	}
}

const CellInfo& Scene::GetCellInfo(Cell cell)
{
	auto find = _grid.find(cell);
	if (find != _grid.end())
	{
		return find->second;	// N개의 Actor List
	}

	// 예외가 발생해서, 관리하고 있지 않는 Cell 정보를 요구한다?
	static CellInfo Empty;	// 스택에 임시로 생성된 변수
	return Empty;
}

// 진짜로 관리하는 actor 컨테이너에 추가
void Scene::addActor(Actor* actor)
{
	actor->Init();

	// 직접 관리하는 _actor 컨테이너에 바로 넣지않는다.
	// Update 로직에서 _actor 컨테이너를 순회중일수도 있기때문에
	// 안전하게 사용하기 위해서 예약 시스템을 사용한다.
	_actors.insert(actor);

	// 렌더링 순서에 맞게, 렌더링 컨테이너도 채워준다.
	if (actor->GetRenderLayer() == RenderLayer::Max)
		return;

	_renderList[(int32)actor->GetRenderLayer()].insert(actor);

	// 충돌체크를 해야하는 액터라면, CollisionManager 등록
	if (actor->IsCheckCollision())
	{
		CollisionManager::GetInstance()->AddCheckCollider(actor->GetCollider());
	}
}

void Scene::removeActor(Actor* actor)
{
	//actor->Destroy();

	// 그리드에서도 제거 
	UpdateGrid(actor, actor->GetPos(), Vector{ -1,-1 });

	// 충돌체크를 해야하는 액터라면, CollisionManager 등록
	if (actor->IsCheckCollision())
	{
		CollisionManager::GetInstance()->RemoveCheckCollider(actor->GetCollider());
	}

	// find 하는 빈도가 잦을수도 있다.
	// 총알이 충돌되거나, 화면밖에 나가면, 총알 객체를 찾아서 _actor 지워야한다.
	//auto it = std::find(_actors.begin(), _actors.end(), actor);

	// 충돌체크를 해야하는 액터라면, CollisionManager 제거
	
	// 렌더링 리스트에서 제거
	if (actor->GetRenderLayer() != RenderLayer::Max)
	{
		auto it = _renderList[(int32)actor->GetRenderLayer()].find(actor);
		if (it != _renderList[(int32)actor->GetRenderLayer()].end())
		{
			_renderList[(int32)actor->GetRenderLayer()].erase(it);
		}
	}

	// 진짜 메모리 해제
	{
		auto it = _actors.find(actor);
		if (it != _actors.end())
		{
			// 씬에서 관리하는 컨테이너에서는 제거 가능 (오브젝트 풀 or new )
			_actors.erase(it);

			// 오브젝트 풀에서 만들어진 Actor라면 풀에 다시 반환
			if (ObjectPool* objectPool = actor->GetObjectPool())
			{
				// 오브젝트 풀 객체
				objectPool->Return(actor);
			}
			else
			{
				actor->Destroy();
				// new 만들어진 객체
				delete actor;	// 메모리 해제
			}
		}
	}
}
