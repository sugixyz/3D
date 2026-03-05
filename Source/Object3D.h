#pragma once
#include "../Library/GameObject.h"

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Draw() override;
protected:
	int hModel;
	VECTOR3 position;
	VECTOR3 rotation;
};