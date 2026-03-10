#pragma once
#include "../Library/GameObject.h"

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Draw() override;

	VECTOR3 GetRotation() { return rotation; }
	VECTOR3 GetPos() { return position; }
	bool isDead() { return HP <= 0; }
	virtual void Attack();
	void TakeDamage(int damage);
protected:
	int hModel;
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 velocity;

	//スタミナ
	float stamina;
	void DrawStamina();

	//HP
	float HP;
	float HP_MAX;
	void DrawHP();

	int damage;
	bool isAttack;
};