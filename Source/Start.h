#pragma once
#include"../Library/GameObject.h"

class Start : public GameObject {
public:
	Start();
	~Start();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 position;
	float maxTime;
	float time;
};