#pragma once
#include"Object3D.h"

class Camera : public Object3D
{
public:
	Camera();
	~Camera();
	void Update() override;
	//SetCamera用に使用
	void Draw() override;
	void SetPlayerPosition(VECTOR3 pos) { lookPos = pos; }
	VECTOR3 Forward();
private:
	VECTOR3 lookPos;
};