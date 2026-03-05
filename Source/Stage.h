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
private:
	int hModel;
};