#pragma once
#include"Object3D.h"

class StageObject : public Object3D
{
public:
	StageObject(int id,VECTOR3 pos,VECTOR3 rot,VECTOR3 sca);
	~StageObject();

	bool CollideRay(VECTOR3 start, VECTOR3 end, VECTOR3* hit);
	VECTOR3 CollideSphere(VECTOR3 center, float radius);
private:
	int hHitModel;
};