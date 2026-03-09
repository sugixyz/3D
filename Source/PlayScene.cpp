#include "PlayScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"

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
	if (Input::IsKeyOnTrig(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", COL_WHITE);
	DrawString(100, 400, "Push [T]Key To Title", COL_WHITE);
}
