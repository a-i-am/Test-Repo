#pragma once

#include "Actor.h"

class Airplane : public Actor
{
	using Super = Actor;
public:
	Airplane(Vector pos);
	virtual ~Airplane();

protected:
	float _speed = 300;
};

