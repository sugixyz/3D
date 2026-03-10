#include "Clear.h"
#include "Lerp.h"
#include"DxLib.h"
#include"Screen.h"

Clear::Clear()
{
	position.x = Screen::WIDTH / 2 - 100;
	position.y = 100;
	maxTime = 2.0f;
	time = 0;
}

Clear::~Clear()
{
}

void Clear::Update()
{
	time += Time::DeltaTime();
	if (time >= maxTime) {
		DestroyMe();
	}
}

void Clear::Draw()
{
	int size = GetFontSize();
	SetFontSize(60);
	DrawString(position.x, position.y, "Clear", COL_WHITE);
	SetFontSize(size);
}
