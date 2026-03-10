#pragma once
#include "Object3D.h"

class PlayerSword : public Object3D
{
public:
	PlayerSword();
	~PlayerSword();
	void Update() override;
	void Draw() override;
private:
	float startTimer;
	float endTimer;

	float rate;
};