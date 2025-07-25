#pragma once

class Missile
{
public:
	void Init(float posX, float posY, float angle, bool chase);
	void Update(float deltaTime);
	void Render(HDC hdc);


private:
	Vector _pos;

	float _speed = 400;
	float _angle = 0;
	bool _chase = false;
};

