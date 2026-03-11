#include "Result.h"
#include"DxLib.h"
#include"Screen.h"

Result::Result()
{
	position.x = 0;
	position.y = 0;
	maxTime = 0.0f;
	time = 0.0f;
	GoblinCounter = 0;
}

Result::Result(int gbCounter)
{
	position.x = Screen::WIDTH / 2 - 100;
	position.y = 100;
	maxTime = 3.0f;
	time = 0.0f;
	GoblinCounter = gbCounter;
}

Result::~Result()
{
}

void Result::Update()
{
	time += Time::DeltaTime();
	if (time >= maxTime) {
		DestroyMe();
	}
}

void Result::Draw()
{
	int size = GetFontSize();
	SetFontSize(60);
	DrawString(position.x, position.y, "RESULT", COL_RED);
	DrawString(position.x, position.y+100, "ì¢î∞êî", COL_WHITE);
	DrawFormatString(position.x - 100, position.y + 180, COL_WHITE, "ÉSÉuÉäÉìÅF%dëÃ", GoblinCounter);
	SetFontSize(size);
}
