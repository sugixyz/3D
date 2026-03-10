#pragma once
#include"../Library/GameObject.h"

class Clear : public GameObject {
public:
	Clear();
	~Clear();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 position;
	float maxTime;
	float time;
};