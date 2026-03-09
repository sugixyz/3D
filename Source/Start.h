#pragma once
#include"../Library/GameObject.h"

class Start : public GameObject {
public:
	Start();
	~Start();
	void Update() override;
	void Draw() override;
private:
	int hImage;
	VECTOR3 position;
	float startX;
	float endX;
	float maxTime;
	float time;
};