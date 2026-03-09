#pragma once
#include"../Library/GameObject.h"

/// <summary>
/// 画面のフェードをするクラス
///				Author : H.Suginuma
/// </summary>
class Fade : public GameObject {
public:
	Fade();
	~Fade();
	void Update()override;
	void Draw() override;

	void FadeIn(float maxFadeTime);
	void FadeOut(float maxFadeTime);
	bool IsFinish();

	void UpdateFadein();
	void UpdateFadeout();
	void DrawFadein();
	void DrawFadeout();
private:
	float maxTime;
	float time;
	unsigned int color;
	int boxCount;
	enum State {
		NON,
		FADE_IN,
		FADE_OUT
	};
	State state;
};