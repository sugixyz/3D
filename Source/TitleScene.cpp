#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (Input::IsKeyOnTrig(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (Input::IsKeyOnTrig(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawString(0, 0, "TITLE SCENE", COL_WHITE);
	DrawFormatString(0, 50, COL_WHITE, "CoG: %s", Version() );
	DrawFormatString(0, 75, COL_WHITE, "DxLib: %s", DXLIB_VERSION_STR_T );
	DrawFormatString(0, 100, COL_WHITE, "FPS: %4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 400, "Push [P]Key To Play", COL_WHITE);
}
