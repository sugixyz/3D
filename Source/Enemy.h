#pragma once
#include "Object3D.h"

class Enemy : public Object3D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
private:
	VECTOR3 velocity;
};