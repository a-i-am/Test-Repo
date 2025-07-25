#pragma once

// 게임에서 한개만 존재하니깐 싱글톤과 비슷하지만, 
// 일일히 GetInstance 하기 귀찮아서
// 카메라는 카메라의 좌표값만 필요해서 굳이 싱글톤으로 하지 않는다.


class Camera
{
public:

	static void Update(float deltaTime, Vector playerPos, Vector mapSize);
	
	static Vector ConvertScreenPos(Vector worldPos);
	static Vector ConvertWorldPos(Vector screenPos);

	static void SetCameraPos(Vector pos) { cameraPos = pos; }
	static Vector GetCameraPos() { return cameraPos; }

private:
	static Vector cameraPos;
};

