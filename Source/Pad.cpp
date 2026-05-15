#include "Pad.h"
#include"../ImGui/imgui.h"

Pad::Pad()
{
	DontDestroyOnSceneChange();
}

Pad::~Pad()
{
}

void Pad::Update()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &inputState);
	//キー入力をコントローラー入力に変換	
	if (CheckHitKey(KEY_INPUT_D))
	{
		inputState.ThumbLX = 32767;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		inputState.ThumbLX = -32768;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		inputState.ThumbLY = 32767;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		inputState.ThumbLY = -32678;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		inputState.ThumbRX = 32767;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		inputState.ThumbRX = -32768;
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		inputState.ThumbRY = 32767;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		inputState.ThumbRY = -32678;
	}
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		inputState.Buttons[ATTACK] = 1;
	}
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		inputState.Buttons[JUMP] = 1;
	}

	ImGui::Begin("PAD");
	int sX = inputState.ThumbLX;
	int sY = inputState.ThumbLY;
	int a = inputState.Buttons[XINPUT_BUTTON_A];
	int b = inputState.Buttons[XINPUT_BUTTON_B];
	ImGui::InputInt("X", &sX);
	ImGui::InputInt("Y", &sY);
	ImGui::InputInt("A", &a);
	ImGui::InputInt("B", &b);
	ImGui::End();
}

float IntToRate(int n)
{
	// -32768～32767を-1～1に変換して返す
	float rate = n / 32767.0f;
	//制限
	if (rate > 1.0f)rate = 1.0f;
	if (rate < -1.0f)rate = -1.0f;
	//中央を無視
	if (-0.2f < rate && rate < 0.2f)rate = 0.0f;
	return rate;
}

float Pad::LStickX()
{
	int sX = inputState.ThumbLX;
	return IntToRate(sX);
}

float Pad::LStickY()
{
	int sY = inputState.ThumbLY;
	return IntToRate(sY);
}

float Pad::RStickX()
{
	int sX = inputState.ThumbRX;
	return IntToRate(sX);
}

float Pad::RStickY()
{
	int sY = inputState.ThumbRY;
	return IntToRate(sY);
}

bool Pad::Pushed(int id)
{
	return inputState.Buttons[id] != 0;
}

