#pragma once
#include "Object3D.h"

class Goblin : public Object3D
{
public:
	Goblin();
	~Goblin();
	void Update() override;
	void Attack() override;
	void PlayStart() { state = State::INPLAY; }
	void KnockBack();
private:
	enum State {
		READY,
		INPLAY,
		STAN
	};
	State state;
	void UpdateInplay();
	void UpdateStan();
};