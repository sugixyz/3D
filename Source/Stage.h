#pragma once
#include "../Library/GameObject.h"

class Stage : public GameObject
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	bool CollideRay(VECTOR3 start, VECTOR3 end, VECTOR3* hit);
	VECTOR3 CollideSphere(VECTOR3 center, float radius);
private:
	int hModel;
	void ReadMappingData(int stageNo);
};