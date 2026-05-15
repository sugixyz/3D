#pragma once
#include"../Library/GameObject.h"

class Pad : public GameObject
{
public:
	Pad();
	~Pad();
	void Update() override;
	//左スティックを横にどれくらい倒しているか
	float LStickX();
	//左スティックを縦にどれくらい倒しているか
	float LStickY();
	//右スティックを横にどれくらい倒しているか
	float RStickX();
	//右スティックを縦にどれくらい倒しているか
	float RStickY();
	//idのボタンを押しているか
	bool Pushed(int id);
	static const int ATTACK = XINPUT_BUTTON_A;
	static const int JUMP = XINPUT_BUTTON_B;
private:
	XINPUT_STATE inputState;
};