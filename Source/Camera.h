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
	void Delay(float time = 0.3f);
	void Shake();
private:
	VECTOR3 lookPos;
	//画角
	float fov;
	//遅延時間	
	float delayTime;
	//遅延の経過時間	
	float delayElapseTime;

	//シェイクの時間
	float shackTime;
	//シェイクの経過時間
	float shackElapseTime;
};