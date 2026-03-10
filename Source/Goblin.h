#pragma once
#include "Object3D.h"

class Goblin : public Object3D
{
public:
	Goblin();
	~Goblin();
	void Update() override;
	void PlayStart() { state = State::INPLAY; }
	void KnockBack();
private:
	VECTOR3 velocity;
	enum State {
		READY,
		INPLAY,
		STAN
	};
	State state;
	void UpdateInplay();
	void UpdateStan();
};