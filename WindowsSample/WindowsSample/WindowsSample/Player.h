#pragma once

// 플레이어 객체가 Game 클래스를 접근해서 뭔가 하고 싶다.
class Player
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

	Vector GetForwardVector();	// 모든 엔진에서도 전방벡터를 Forward Vector

private:
	//vector<pair<POINT, POINT>> _lines;	// 내 비행기의 모습을 나타내는 라인정보
	class LineMesh* _lineMesh = nullptr;
	class TextureResource* _texture = nullptr;	// 리소스 매니저가 가지고 있는 원본 텍스처





	Vector _pos;

	float _speed = 300.0f;	// 1초당 이동하는 속도
	float _angle = 0;	// 포신의 각도(누적을 위해서 멤버변수로 저장)

	float _FOV = 0;	// 시야각
};

