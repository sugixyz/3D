#include "PlayScene.h"
#include "Player.h"
#include "Goblin.h"
#include "Stage.h"
#include"DxLib.h"
#include"Start.h"
#include"Clear.h"
#include"Over.h"
#include"Fade.h"
#include"CountDown.h"
#include"PlayerSword.h"

PlayScene::PlayScene()
{
	Fade* fade = new Fade();
	new Stage();
	new Player();
	new Goblin();
	state = State::COUNT;
	gTime = 0;
	fade->FadeIn(1.0);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	switch (state) {
	case COUNT: 
	{
		if (gTime < 1.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 1.0f) {
				new CountDown();
				state = State::START;
				gTime = 0;
			}
		}
		break;
	}
	case START: 
	{
		//カウントダウン終了まで待つ
		if (gTime < 3.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 3.0f) {
				new Start();
				Player* pl = FindGameObject<Player>();
				pl->PlayStart();
				Goblin* gb = FindGameObject<Goblin>();
				gb->PlayStart();
				//new PlayerSword();
				state = State::INPLAY;
				gTime = 0;
			}
		}
		break;
	}
	case INPLAY: 
	{
		if (gTime < 5.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 5.0f) {
				Goblin* e = new Goblin();
				e->PlayStart();
				gTime = 0;
			}
		}
		break;
	}
	case OVER:
	case CLEAR:
	{
		if (gTime < 1.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 1.0f) {
				Fade* fade = FindGameObject<Fade>();
				fade->FadeOut(1.5);
				state = State::END;
			}
		}
		break;
	}
	case END: 
	{
		Fade* fade = FindGameObject<Fade>();
		if (fade->IsFinish()) {
			SceneManager::ChangeScene("TITLE");
		}
		break;
	}
	}

	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	//DrawFormatString(0, 40, COL_WHITE, "SCENE_%d", state);
}

void PlayScene::OnClear()
{
	if (state == State::INPLAY) {
		state = State::CLEAR;
		new Clear();
		gTime = 0;
	}
}

void PlayScene::OnOver() {

	if (state == State::INPLAY) {
		state = State::OVER;
		new Over();
		gTime = 0;
	}
}
