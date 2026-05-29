#include "PlayScene.h"
#include "Player.h"
#include "Goblin.h"
#include "Stage.h"
#include"DxLib.h"
#include"Start.h"
#include"Fade.h"
#include"CountDown.h"
#include"Result.h"
#include"Camera.h"
#include"Pad.h"

PlayScene::PlayScene()
{
	Fade* fade = new Fade();
	new Stage();
	//new Player();
	//new Goblin();
	new Camera();

	state = State::COUNT;
	gTime = 0;
	GoblinCounter = 0;
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
				auto gb = FindGameObjects<Goblin>();
				for (auto g : gb)
				{
					g->PlayStart();
				}
				state = State::INPLAY;
				gTime = 0;
			}
		}
		break;
	}
	case INPLAY: 
	{
		break;
	}
	case RESULT:
	{
		if (gTime < 3.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 3.0f) {
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

void PlayScene::OnResult()
{
	if (state == State::INPLAY) {
		state = State::RESULT;
		new Result(GoblinCounter);
		gTime = 0;
	}
}
