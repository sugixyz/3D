#pragma once
#include "Object3D.h"

class Player : public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
private:
};