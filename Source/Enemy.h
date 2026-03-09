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
	float VectorLength(const VECTOR3& a, const VECTOR3& b);
};