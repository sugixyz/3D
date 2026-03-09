#include "Start.h"
#include "Lerp.h"
#include"DxLib.h"

Start::Start()
{
	position.x = 200;
	position.y = 400;
	startX = 800;
	endX = 200;
	maxTime = 5.0f;
	time = 0;
}

Start::~Start()
{
}

void Start::Update()
{
	time += Time::DeltaTime();
	float rate = time / maxTime;
	position.x = Lerp<float>(startX, endX, rate, easeInSine);
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
