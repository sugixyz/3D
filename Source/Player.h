#pragma once
#include "Object3D.h"

class Player : public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
	bool IsJump() { return isJump; }
	void PlayStart() { state = State::INPLAY; }
private:
	VECTOR3 velocity;
	bool isJump;
	enum State {
		READY,
		INPLAY,
		OVER
	};
	State state;
	void UpdateReady();
	void UpdateInplay();
	void UpdateOver();

	//スタミナ
	float stamina;
	//スタミナ切れ
	bool isExhausted;
	void PlayerExhausted();
};