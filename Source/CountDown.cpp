#include "CountDown.h"
#include "Lerp.h"
#include"DxLib.h"
#include"Screen.h"

CountDown::CountDown()
{
	position.x = Screen::WIDTH / 2;
	position.y = 100;
	maxTime = 3.0f;
	time = 0;

	startSize = 40;
	endSize = 80;
	currentSize = 40;

	count = 3;
}

CountDown::~CountDown()
{
}

void CountDown::Update()
{
	time += Time::DeltaTime();
	float rate = time / 1.0;
	if (rate >= 1.0) {
		count--;
		time = 0;
	}
	//count = (int)(maxTime - time) + 1;
	currentSize = Lerp<float>(startSize, endSize, rate, easeInSine);
	if (count == 0) {
		DestroyMe();
	}
}

void CountDown::Draw()
{
	int size = GetFontSize();
	SetFontSize(currentSize);
	float x, y;
	x = position.x - (currentSize / 2);
	y = position.y - (currentSize / 2);
	DrawFormatString(x, y, COL_YELLOW, "%d",count);
	SetFontSize(size);
}
