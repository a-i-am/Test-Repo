#include "pch.h"
#include "LobbyScene.h"
#include "ResourceManager.h"
#include "UIButton.h"
#include "Game.h"

void LobbyScene::Init()
{
	Super::Init();

	// UI 구성
	_ui.CreateImage(Vector(0, 0), "LobbyBG");
	_ui.CreateImage(Vector(20, 20), "Logo");

	UIButton* playButton = _ui.CreateButton(Vector(10, 300), "Btn_Play");
	// 함수 포인터 연결
	playButton->SetClickEvent([this]() { onClickPlayButton(); });

	UIButton* mapEditorButton = _ui.CreateButton(Vector(10, 500), "Btn_MapEditor");
	// 함수 포인터 연결
	mapEditorButton->SetClickEvent([this]() { onClickMapEditorButton(); });
}

void LobbyScene::Destroy()
{
	Super::Destroy();
}

void LobbyScene::Update(float deltatTime)
{
	Super::Update(deltatTime);
}

void LobbyScene::Render(HDC hdc)
{
	Super::Render(hdc);
}

void LobbyScene::loadResource()
{
	ResourceManager::GetInstance()->LoadTexture("LobbyBG", L"background_title.bmp");
	ResourceManager::GetInstance()->LoadTexture("Btn_MapEditor", L"mapEditor.bmp");
	ResourceManager::GetInstance()->LoadTexture("Btn_Play", L"play.png");
	ResourceManager::GetInstance()->LoadTexture("Logo", L"title.png");
}

void LobbyScene::onClickPlayButton()
{
	Game::GetInstance()->ChangeGameScene();
}

void LobbyScene::onClickMapEditorButton()
{
	Game::GetInstance()->ChangeEditorScene();
}
