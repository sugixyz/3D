#pragma once
#include"Object3D.h"

class Light : public Object3D
{
public:
	Light();
	~Light();
	void Update() override;
private:
};
