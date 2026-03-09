#pragma once
#include"../Library/GameObject.h"

class CountDown : public GameObject {
public:
	CountDown();
	~CountDown();
	void Update() override;
	void Draw() override;
private:
	VECTOR3 position;
	float maxTime;
	float time;
	//サイズ
	int startSize;
	int endSize;
	int currentSize;

	//カウント
	int count;
};