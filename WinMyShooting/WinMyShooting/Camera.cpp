#include "pch.h"
#include "Camera.h"

Vector Camera::cameraPos = Vector(0, 0);

// 시작은 카메라가 항상 플레이어를 따라서 움직인다.
void Camera::Update(float deltaTime, Vector playerPos, Vector mapSize)
{
	//cameraPos = playerPos;
	cameraPos.y -= 100 * deltaTime;

	// 카메라가 Window Size 화면밖으로 못나가게 min, max 값을 자른다.
	// 이떄 카메라는 화면의 정중앙에 위치해야하니깐, Window Size / 2 만큼 못나가게 막으면 된다.
	float halfSizeX = GWinSizeX / 2;
	float halfSizeY = GWinSizeY / 2;

	cameraPos.x = clamp(cameraPos.x, halfSizeX, mapSize.x - halfSizeX);
	cameraPos.y = clamp(cameraPos.y, halfSizeY, mapSize.y - halfSizeY);
}

Vector Camera::ConvertScreenPos(Vector worldPos)
{
	Vector convertPos;

	convertPos.x = worldPos.x - (cameraPos.x - (GWinSizeX / 2));
	convertPos.y = worldPos.y - (cameraPos.y - (GWinSizeY / 2));

	return convertPos;
}

// 화면 좌표계 기준으로 픽킹한 위치는 
// 카메라가 어디쯤에 있느냐에 따라서 좌표값이 월드 좌표계의 값과 다르다.
// 카메라 기준으로 World 좌표로 변환을 해줘야 
// 의미있는 픽킹 or 마우스 클릭 위치 충돌체크 할수 있다.
Vector Camera::ConvertWorldPos(Vector screenPos)
{
	Vector convertPos;

	convertPos.x = screenPos.x + (cameraPos.x - (GWinSizeX / 2));
	convertPos.y = screenPos.y + (cameraPos.y - (GWinSizeY / 2));

	return convertPos;
}
