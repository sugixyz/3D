#include "Over.h"
#include "Lerp.h"
#include"DxLib.h"
#include"Screen.h"

Over::Over()
{
	position.x = Screen::WIDTH / 2 - 100;
	position.y = 100;
	maxTime = 2.0f;
	time = 0;
}

Over::~Over()
{
}

void Over::Update()
{
	time += Time::DeltaTime();
	if (time >= maxTime) {
		DestroyMe();
	}
}

void Over::Draw()
{
	int size = GetFontSize();
	SetFontSize(60);
	DrawString(position.x, position.y, "Over", COL_WHITE);
	SetFontSize(size);
}
