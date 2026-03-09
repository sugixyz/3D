#pragma once
#include "Object3D.h"

class Enemy : public Object3D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void PlayStart() { state = State::INPLAY; }
private:
	VECTOR3 velocity;
	enum State {
		READY,
		INPLAY,
		OVER
	};
	State state;
	void UpdateReady();
	void UpdateInplay();
	void UpdateOver();
	float VectorLength(const VECTOR3& a, const VECTOR3& b);
};