#pragma once
#include "Object3D.h"

class Player : public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
	bool IsJump() { return isJump; }
private:
	VECTOR3 velocity;
	bool isJump;
	//VECTOR3 camRotation;
	//VECTOR3 mousePos;
};