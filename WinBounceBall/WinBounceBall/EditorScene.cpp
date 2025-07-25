#include "pch.h"
#include "EditorScene.h"
#include "InputManager.h"
#include "Ball.h"
#include "Block.h"
#include "Star.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Game.h"
#include "StageLoader.h"

void EditorScene::Init()
{
	Super::Init();

	InputManager::GetInstance()->SetEventMouseWheel([this](int32 delta) 
		{ 
			OnMouseWheel(delta); 
		});

	// 각 편집모드마다 임시 객체의 정보가 다르다.
	// ball
	{
		SELECT_MODE mode = SELECT_MODE::BALL;
		Actor* actor = new Ball(Vector(0, 0));
		actor->Init();

		_editActorInfo[(int32)mode].tempActor = actor;
		_editActorInfo[(int32)mode].spriteIndex = 0;
	}
	// block
	{
		SELECT_MODE mode = SELECT_MODE::BLOCK;
		Actor* actor = new Block(Vector(0, 0));
		actor->Init();

		_editActorInfo[(int32)mode].tempActor = actor;
		_editActorInfo[(int32)mode].spriteIndex = 0;
	}
	// star
	{
		SELECT_MODE mode = SELECT_MODE::STAR;
		Actor* actor = new Star(Vector(0, 0));
		actor->Init();

		_editActorInfo[(int32)mode].tempActor = actor;
		_editActorInfo[(int32)mode].spriteIndex = 0;
	}
}

void EditorScene::Destroy()
{
	InputManager::GetInstance()->SetEventMouseWheel(nullptr);

	Super::Destroy();

	for (auto info : _editActorInfo)
	{
		SAFE_DELETE(info.tempActor);
	}
}

void EditorScene::Update(float deltatTime)
{
	Super::Update(deltatTime);

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F1))
	{
		_currMode = SELECT_MODE::BLOCK;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::F2))
	{
		_currMode = SELECT_MODE::BALL;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::F3))
	{
		_currMode = SELECT_MODE::STAR;
	}
	// 마우스 클릭시, 현재 모드(블럭/공/별)에 따라서 객체를 Scene 추가한다.
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		int32 x = (mousePos.x / BLOCK_SIZE);
		int32 y = (mousePos.y / BLOCK_SIZE);

		// x, y 좌료를 64단위로 잘라서 위치 계산
		x = x * BLOCK_SIZE + (int32)(BLOCK_SIZE * 0.5f);
		y = y * BLOCK_SIZE + (int32)(BLOCK_SIZE * 0.5f);

		Actor* newActor = nullptr;
		if (_currMode == SELECT_MODE::BLOCK)
		{
			newActor = new Block(Vector(x, y));
		}
		else if (_currMode == SELECT_MODE::BALL)
		{
			// 공은 씬에 한개만 존재해야해서, 씬에 추가된 Actor를 순회하면서, 공 객체를 삭제 예약한다.
			for (auto iter : _actors)
			{
				if (dynamic_cast<Ball*>(iter))
				{
					ReserveRemove(iter);
				}
			}
			newActor = new Ball(Vector(x, y));
		}
		else if (_currMode == SELECT_MODE::STAR)
		{
			newActor = new Star(Vector(x, y));
		}

		ReserveAdd(newActor);

		auto& info = _editActorInfo[(int32)_currMode];

		int32 indexX, indexY;

		// 마우스를 따라다니는 오브젝트의 스프라이트 정보를 그대로 가져온다.
		{
			SpriteRenderer* spriteRenderer = info.tempActor->GetComponent<SpriteRenderer>();
			if (spriteRenderer == nullptr)
			{
				return;
			}
			spriteRenderer->GetIndex(indexX, indexY);
		}

		// 새로 생성되는 객체의 sprite index 를 동일하게 맞춰준다.
		{
			SpriteRenderer* spriteRenderer = newActor->GetComponent<SpriteRenderer>();
			spriteRenderer->SetSpriteIndex(indexX, indexY);
		}
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::S))
	{
		Save();
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::L))
	{
		Load();
	}
}

void EditorScene::Render(HDC hdc)
{
	Super::Render(hdc);

	// 가로선 그리기
	for (int y = 0; y <= GWinSizeY; y += BLOCK_SIZE)
	{
		MoveToEx(hdc, 0, y, nullptr);
		LineTo(hdc, GWinSizeX, y);
	}

	// 세로선 그리기
	for (int x = 0; x <= GWinSizeX; x += BLOCK_SIZE)
	{
		MoveToEx(hdc, x, 0, nullptr);
		LineTo(hdc, x, GWinSizeY);
	}

	POINT mousePos = InputManager::GetInstance()->GetMousePos();

	// 마우스를 따라다니는 오브젝트 렌더링
	_editActorInfo[(int32)_currMode].tempActor->SetPos(Vector(mousePos.x, mousePos.y));
	_editActorInfo[(int32)_currMode].tempActor->Render(hdc);
}

void EditorScene::loadResource()
{
	ResourceManager::GetInstance()->LoadTexture("Ball", L"001-008_ball.bmp", 2, 4);
	ResourceManager::GetInstance()->LoadTexture("Block", L"201-217_block.bmp", 3, 6);
	ResourceManager::GetInstance()->LoadTexture("Star", L"101-103_star.bmp", 1, 3);
}

void EditorScene::OnMouseWheel(int32 delta)
{
	// 현재 편집중인 임시객체의 SPrite INdex 를 변경
	auto& info = _editActorInfo[(int32)_currMode];

	SpriteRenderer* spriteRenderer = info.tempActor->GetComponent<SpriteRenderer>();
	if (spriteRenderer == nullptr)
	{
		return;
	}

	int32 maxCount = spriteRenderer->GetMaxFrameCount();

	if (delta > 0)
	{
		info.spriteIndex++;
		if (info.spriteIndex >= maxCount)
			info.spriteIndex = 0;
	}
	else
	{
		info.spriteIndex--;
		if (info.spriteIndex < 0) 
			info.spriteIndex = maxCount - 1;
	}

	// Ball_0,
	// Ball_1
	string spriteKey = std::format("{0}_{1}", spriteRenderer->GetSpriteKey(), info.spriteIndex);
	auto spriteInfo = ResourceManager::GetInstance()->GetSpriteInfo(spriteKey);

	spriteRenderer->SetSpriteIndex(spriteInfo->startX, spriteInfo->startY);
}

void EditorScene::Save()
{
	// 라인메시 비행기 그렸을떄와 코드가 동일한다. 내용물만 다릅니다.

	// 어디에 저장할지 윈도우 창 띄우기
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Game::GetInstance()->GetHwnd();
	ofn.lpstrFilter = L"스테이지 파일 (*.stage)\0*.stage\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"stage";

	if (GetSaveFileName(&ofn))
	{
		// 파일 이름이 선택되었으면 저장
		std::wstring fileName = szFileName;

		std::wofstream file(fileName);
		if (file.is_open())
		{
			// 모든 씬을 순회하면서 필요한 데이터 저장
			for (auto iter : _actors)
			{
				iter->SaveActor(file);
			}

			file.close();
			MessageBox(Game::GetInstance()->GetHwnd(), L"맵이 저장되었습니다.", L"저장 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(Game::GetInstance()->GetHwnd(), L"파일을 저장할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}

void EditorScene::Load()
{
	// 파일 열기 윈도우창 생성
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Game::GetInstance()->GetHwnd();
	ofn.lpstrFilter = L"스테이지 파일 (*.stage)\0*.stage\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"stage";

	if (GetOpenFileName(&ofn))
	{
		// 파일 이름이 선택되었으면 로드
		std::wstring fileName = szFileName;

		std::wifstream file(fileName);
		if (file.is_open())
		{
			StageLoader loader;
			loader.Load(this, file);

			file.close();
			MessageBox(Game::GetInstance()->GetHwnd(), L"맵이 로드되었습니다.", L"로드 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(Game::GetInstance()->GetHwnd(), L"파일을 로드할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}
