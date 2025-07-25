#include "pch.h"
#include "Player.h"
#include "Game.h"
#include "InputManager.h"
#include "Enemy.h"
#include "GameScene.h"
#include "LineMesh.h"
#include "TextureResource.h"
#include "ResourceManager.h"

void Player::Init()
{
	_pos.x = 100;
	_pos.y = 100;

	_angle = DegreeToRadian(90);
	_FOV = DegreeToRadian(60);

	// 생성될때, 우리의 파일을 로드해서 Line정보를 다 들고있자.
	//file load -> 상위클래스로 올려서. 적 비행기도 관리하자.
	// 총알도 우리만든 비행기모습으로 그리고 싶다.
	// 
	// lines.push_back( 시작/끝 )
	//_lineMesh = new LineMesh();	// 공유되는 리소스 개념X, 
	//_lineMesh = ResourceManager::GetInstance()->GetLineMesh("Player");

	_texture = ResourceManager::GetInstance()->GetTexture("Player");

}

void Player::Update(float deltaTime)
{
	GameScene* gameScene = GameScene::GetGameScene();
	if (gameScene == nullptr)
		return;

	// W 키가 눌러졌으면 위로 이동.
	// S 키가 눌러졌으면 아래로 이동.
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		// 이동 거리 = 시간 * 속도
		// 1초당 10만큼 이동한다.
		_pos.y -= (deltaTime * _speed);	// 모든 PC가 1프레임당 동일한 거리를 이동한다.
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		_pos.y += (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		_pos.x -= (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		_pos.x += (deltaTime * _speed);
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::Q))
	{
		// 10 값 : 1초에 회전하는 각도
		_angle += (deltaTime * 10);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::E))
	{
		_angle -= (deltaTime * 10);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::SpaceBar))
	{
		float firePosX = _pos.x + 40 * ::cos(_angle);
		float firePosY = _pos.y - 40 * ::sin(_angle);

		// 스페이스바를 눌렀을때, 몬스터와의 각도가 시야각도 안인지 판단.
		Vector enemyPos = gameScene->GetEnemy()->GetPos();

		Vector dirToEnemy = enemyPos - _pos;	// 플레이어의 위치 -> 마우스 위치까지 가는 방향벡터를 얻는다
		dirToEnemy.Normalize();	// 크기를 없애고 방향만 남긴다.

		// 전방 벡터 구하기
		Vector forward = GetForwardVector();
		float dot = forward.Dot(dirToEnemy);
		float radianDot = acos(dot);	// 

		// _FOV 라디안 값
		if (radianDot < _FOV / 2)
		{
			// 몬스터가 시야내에 있다.
			gameScene->CreateMissile(firePosX, firePosY, _angle, true);
		}
		else
		{
			// 없다.
			gameScene->CreateMissile(firePosX, firePosY, _angle, false);
		}
	}
}

void Player::Render(HDC hdc)
{
	// 원 그리기
	// left, right, top, bottom
	// _pos.x, _poY 가 중심이 되도록, 그림을 그릴때 좌표를 조정해준다.
	//::Ellipse(hdc, (int32)_pos.x - 25, (int32)_pos.y - 25, (int32)_pos.x + 25, (int32)_pos.y + 25);

	if (_lineMesh)
	{
		_lineMesh->Render(hdc, _pos);
	}

	// 텍스처 그리기
	if (_texture)
	{
		_texture->Render(hdc, _pos);
	}
	

	// 현재 포신의 각도가 궁금한데
	{
		wstring str = std::format(L"Angle({0})", RadianToDegree(_angle));
		::TextOut(hdc, 500, 500, str.c_str(), static_cast<int32>(str.size()));
	}

	// 전방 벡터 구하기
	Vector forward = GetForwardVector();
	Vector mouseVec(InputManager::GetInstance()->GetMousePos());
	Vector dirToMousePos = mouseVec - _pos;	// 플레이어의 위치 -> 마우스 위치까지 가는 방향벡터를 얻는다
	dirToMousePos.Normalize();	// 크기를 없애고 방향만 남긴다.

	// 몬스터까지의 각도가 궁금한데
	{
		// 마우스 좌표를 몬스터 위치라고 생각하고, 
		// 마우스를 움직여가면서 내적의 값이 어떻게 변화하는지 확인을 해보자.
		float dot = forward.Dot(dirToMousePos);
		float radianDot = acos(dot);

		wstring str = std::format(L"Dot({0}), Angle({1})", dot, RadianToDegree(radianDot));
		::TextOut(hdc, 500, 400, str.c_str(), static_cast<int32>(str.size()));

		// 나의 시야각이 60도. RadianToDegree(radianDot) 이값이 60인지만 확인하면 된다.
		// -60 같은건 안넘어온다. cosA 값이라서 양수에 대한값만 넘어온다.

		// 마우스 위치까지의 라인도 그려보자.
		//::MoveToEx(hdc, (int32)_pos.x, (int32)_pos.y, nullptr);
		//::LineTo(hdc, (int32)mouseVec.x, (int32)mouseVec.y);
	}

	// 포신 그리기
	// -1 ~ 1 사이로 나온다. 실제 포신 길이만큼, 좌표를 이동해야하기 떄문에, * 50(길이)
	float firePosX = _pos.x + ::cos(_angle) * 100;
	float firePosY = _pos.y - ::sin(_angle) * 100;

	// 양옆으로 시야각 라인 표시
	{
		// 라인 기준 색상을 변경하고 싶다.
		DWORD color = RGB(255, 0, 0);
		// 전방 벡터 기준으로 왼/오 마우스 위치에 따라 색상을 바꾸자.
		float cross = dirToMousePos.Cross(forward);
		if (cross > 0)	// 마우스 벡터를 시게방향으로 회전하면 전방 벡터이다.
		{	
			// 마우스 벡터가, 전방벡터의 왼쪽에 있다.
			color = RGB(0, 255, 0);
		}
		else
		{
			color = RGB(0, 0, 255);
		}

		HPEN pen = ::CreatePen(PS_SOLID, 1, color);
		HPEN oldPen = (HPEN)::SelectObject(hdc, pen);

		// 1) 벡터를 회전시키는 방법
		Vector rightFOV = _pos + (forward.Rotate(_FOV / 2) * 50);

		// 2) 각도를 직접 계산해서 원하는 좌표를 계산
		//float rightPosX = _pos.x + ::cos(_angle + (_FOV / 2)) * 50;
		//float rightPosY = _pos.y - ::sin(_angle + (_FOV / 2)) * 50;
		::MoveToEx(hdc, (int32)_pos.x, (int32)_pos.y, nullptr);
		::LineTo(hdc, (int32)rightFOV.x, (int32)rightFOV.y);
		
		Vector leftFOV = _pos + (forward.Rotate(-_FOV / 2) * 50);
		//float leftPosX = _pos.x + ::cos(_angle - (_FOV / 2)) * 50;
		//float leftPosY = _pos.y - ::sin(_angle - (_FOV / 2)) * 50;
		::MoveToEx(hdc, (int32)_pos.x, (int32)_pos.y, nullptr);
		::LineTo(hdc, (int32)leftFOV.x, (int32)leftFOV.y);

		::SelectObject(hdc, oldPen);
		::DeleteObject(pen);
	}


	// 라인 기준 색상을 변경하고 싶다.
	// 매번 그림 그릴때마다 빨간색 펜 생성/삭제/생성/삭제
	{
		DWORD color = RGB(255, 0, 0);
		HPEN pen = ::CreatePen(PS_SOLID, 1, color);
		HPEN oldPen = (HPEN)::SelectObject(hdc, pen);

		// 100 대신에, 포신의 각도에 따라서 좌표를 새로 구해야 한다.
		::MoveToEx(hdc, (int32)_pos.x, (int32)_pos.y, nullptr);
		::LineTo(hdc, (int32)firePosX, (int32)firePosY);

		::SelectObject(hdc, oldPen);
		::DeleteObject(pen);
	}
}

Vector Player::GetForwardVector()
{
	// 1) 플레이어의 위치 -> 포신의 끝위치까지 가는 방향 벡터를 구하면, 그게 전방
	Vector playerPos = _pos;
	Vector firePos;
	firePos.x = _pos.x + ::cos(_angle) * 50;
	firePos.y = _pos.y - ::sin(_angle) * 50;

	Vector dir = firePos - playerPos;
	dir.Normalize();


	// 2) 각도만 가지고, 전방벡터를 구할수도 있다.
	//Vector dir(cos(_angle), -sin(_angle));

	return dir;
}
