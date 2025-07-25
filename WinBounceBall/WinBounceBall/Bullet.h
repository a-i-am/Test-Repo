#pragma once

class Bullet
{
public:
	void Init(Vector pos);
	void Update(float deltaTime);
	void Render(HDC hdc);

private:
	Vector _pos;
	float _speed = 300;

	//class Texture* _texture = nullptr;
};