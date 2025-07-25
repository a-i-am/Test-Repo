#pragma once

class Actor;

struct CellInfo
{
	set<class Actor*> actorList;	// 셀 하나가 관리하는 액터 개수 N
};

// 각 장면을 관리한다.
// LobbyScene, EditScene, GameScene, ChracterSelectScene
class Scene
{
public:
	virtual ~Scene() {}
	virtual void Init();
	virtual void Destroy();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hdc);

	void ReserveRemove(Actor* actor);
	void UpdateGrid(Actor* actor, Vector prevPos, Vector newPos);
	const CellInfo& GetCellInfo(Cell cell);

	// 충돌체크를 위해서 각 레이어별로 반환
	const unordered_set<class Actor*>& GetRenderList(RenderLayer layer) { return _renderList[(int32)layer]; }
	
	// 그리드 디버깅 정보
	static bool drawDebugCell;

	int32 GetMapSizeX() { return _mapSizeX; }
	int32 GetMapSizeY() { return _mapSizeY; }

protected:
	virtual void loadResource() {}
	virtual void createBackgroundObject() {}
	virtual void createDefaultObject() {}

	void addActor(Actor* actor);
	void removeActor(Actor* actor);

	void drawGrid(HDC hdc);
	
protected:
	// 로비씬에서도 액터개념으로 뭔가 그림이 그려져야할수도 있어서
	unordered_set<Actor*> _actors;	// 원본 객체가 있는 개념

	// 렌더링 순서를 보장해서 그리기 위해
	unordered_set<class Actor*> _renderList[(int32)RenderLayer::Max];

	// 그리드, 클래스로 분리해도 된다.
	// 씬이 모든 액터들을 관리하고 있어서, Cell 단위로 액터를 관리하는 시스템
	map<Cell, CellInfo> _grid;	// key : cell (index), data : cell 에서 관리하는 정보들
	int32 _gridCountX = 0;
	int32 _gridCountY = 0;
	//vector<vector<int>>

	// 추가 예정인 액터들을 관리
	vector<Actor*> _reserveAdd;		// 추가되어야 하는 액터들을 관리
	vector<Actor*> _reserveRemove;	// 삭제되어야 하는 액터들을 관리

	// 디폴트는 윈도우 사이즈가 맵사이즈, or 텍스처의 사이즈
	int32 _mapSizeX = GWinSizeX;
	int32 _mapSizeY = GWinSizeY;
};

