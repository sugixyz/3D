#include "Start.h"
#include "Lerp.h"
#include"DxLib.h"
#include"Screen.h"

Start::Start()
{
	position.x = Screen::WIDTH / 2 - 100;
	position.y = 100;
	maxTime = 2.0f;
	time = 0;
}

Start::~Start()
{
}

void Start::Update()
{
	time += Time::DeltaTime();
	if (time >= maxTime) {
		DestroyMe();
	}
}

void Start::Draw()
{
	int size = GetFontSize();
	SetFontSize(60);
	DrawString(position.x, position.y, "START", COL_WHITE);
	SetFontSize(size);
}
