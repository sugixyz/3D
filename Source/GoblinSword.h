#pragma once
#include "Object3D.h"

class GoblinSword : public Object3D
{
public:
	GoblinSword();
	GoblinSword(VECTOR3 pos,VECTOR3 rot);
	~GoblinSword();
	void Update() override;
	void Draw() override;
private:
	float startTimer;
	float endTimer;

	float rate;

	VECTOR3 gbPosition;
	VECTOR3 gbRotation;
};