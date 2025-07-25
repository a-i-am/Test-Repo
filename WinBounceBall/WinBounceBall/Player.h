#pragma once


class Player
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

private:
	Vector _pos;
	float _speed = 300;

	class Texture* _texture = nullptr;
};

