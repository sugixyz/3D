#pragma once
#include"../Library/GameObject.h"

class Result : public GameObject {
public:
	Result();
	Result(int gbCounter);
	~Result();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 position;
	float maxTime;
	float time;

	int GoblinCounter;
};