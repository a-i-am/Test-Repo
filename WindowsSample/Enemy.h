#pragma once

// Texture  부모클래스 만들고, 상속받으면 안되나요?
// 

class Enemy
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

	Vector GetPos() { return _pos; }

private:
	//vector<pair<POINT, POINT>> _lines;	// 적 비행기의 모습을 나타내는 라인정보
	class TextureResource* _texture = nullptr;	// 리소스 매니저가 가지고 있는 원본 텍스처

	Vector _pos;

	Vector _start;
	Vector _end;
};

