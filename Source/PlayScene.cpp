#include "PlayScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include"DxLib.h"
#include"Start.h"

PlayScene::PlayScene()
{
	new Player();
	new Enemy();
	new Stage();
	state = State::READY;
	gTime = 0;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	switch (state) {
	case READY: 
	{
		if (gTime < 1.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 1.0f) {
				//‰¼•\Ž¦
				new Start();
				Player* pl = FindGameObject<Player>();
				pl->PlayStart();
				Enemy* en = FindGameObject<Enemy>();
				en->PlayStart();
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
	case OVER:
	case CLEAR:
	{
		if (gTime < 1.0f) {
			gTime += Time::DeltaTime();
			if (gTime >= 1.0f) {
				//SceneManager::ChangeScene("TITLE");
				state = State::END;
			}
		}
		break;
	}
	case END: 
	{
		SceneManager::ChangeScene("TITLE");
		break;
	}
	}

	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", COL_WHITE);
	DrawFormatString(0, 40, COL_WHITE, "SCENE_%d", state);
	DrawString(100, 400, "Push [T]Key To Title", COL_WHITE);
}

void PlayScene::OnClear()
{
	if (state == State::INPLAY) {
		state = State::CLEAR;
		gTime = 0;
	}
}

void PlayScene::OnOver() {

	if (state == State::INPLAY) {
		state = State::OVER;
		gTime = 0;
	}
}
