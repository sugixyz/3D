#pragma once
#include"../Library/GameObject.h"

class Over : public GameObject {
public:
	Over();
	~Over();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 position;
	float maxTime;
	float time;
};