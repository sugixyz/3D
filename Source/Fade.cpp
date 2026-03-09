#include "Fade.h"
#include"Screen.h"

namespace {
	const int BOX_NUM = 10;
	const float BOX_SIZE = Screen::WIDTH / BOX_NUM;
}

Fade::Fade()
{
	SetDrawOrder(-1000000);
	time = 1.0f;
	maxTime = 1.0f;
	color = GetColor(0, 0, 0);

	boxCount = 0;
	state = NON;
}

Fade::~Fade()
{
}

void Fade::Update()
{
	switch (state) {
	case FADE_IN:
	{
		UpdateFadein();
		break;
	}
	case FADE_OUT:
	{
		UpdateFadeout();
		break;
	}
	}

}

void Fade::UpdateFadein()
{
	if (time < maxTime) {
		time += Time::DeltaTime();
		float rate = time / maxTime;
		boxCount = BOX_NUM - (int)(rate * 10);
	}
}

void Fade::UpdateFadeout()
{
	if (time < maxTime) {
		time += Time::DeltaTime();
		float rate = time / maxTime;
		boxCount = (int)(rate * 10);
	}
}

void Fade::Draw()
{
	switch (state) {
	case FADE_IN:
	{
		DrawFadein();
		break;
	}
	case FADE_OUT:
	{
		DrawFadeout();
		break;
	}
	}
	
}

void Fade::DrawFadein()
{
	if (IsFinish())return;
	for (int i = boxCount; i > 0 - 1; i--) {
		int x, y;
		x = Screen::WIDTH / BOX_NUM * i;
		y = Screen::HEIGHT;
		if (i == boxCount) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(x, 0, x + BOX_SIZE, y, color, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			DrawBox(x, 0, x + BOX_SIZE, y, color, TRUE);
		}
	}
}

void Fade::DrawFadeout()
{
	//if (IsFinish())return;
	for (int i = 0; i < boxCount + 1; i++) {
		int x, y;
		x = Screen::WIDTH / BOX_NUM * i;
		y = Screen::HEIGHT;
		if (i == boxCount) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(x, 0, x + BOX_SIZE, y, color, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			DrawBox(x, 0, x + BOX_SIZE, y, color, TRUE);
		}
	}
}

void Fade::FadeIn(float maxFadeTime)
{
	time = 0;
	boxCount = 0;
	maxTime = maxFadeTime;
	state = State::FADE_IN;
}

void Fade::FadeOut(float maxFadeTime)
{
	time = 0;
	boxCount = 0;
	maxTime = maxFadeTime;
	state = State::FADE_OUT;
}


bool Fade::IsFinish()
{
	//if (time >= maxTime)return true;
	//return false;

	return (time >= maxTime);
}
