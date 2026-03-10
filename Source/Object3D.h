#pragma once
#include "../Library/GameObject.h"

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Draw() override;

	VECTOR3 GetPos() { return position; }
	bool isDead() { return HP <= 0; }
	virtual void Attack();
	virtual void TakeDamage(int damage);
protected:
	int hModel;
	VECTOR3 position;
	VECTOR3 rotation;


	//スタミナ
	float stamina;
	void DrawStamina();

	//HP
	float HP;
	void DrawHP();
};