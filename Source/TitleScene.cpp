#include "TitleScene.h"
#include"Pad.h"
#include<fstream>

TitleScene::TitleScene()
{
	//ファイルを開く out バイナリーファイルとして
	std::ofstream fs("data/test.bin",std::ios::binary);
	struct STR
	{
		int i1;
		int i2;
		char c1;
		char c2;
	};
	STR str;
	str.i1 = 1024;
	str.i2 = 255;
	int size = sizeof(str);
	//書き込み　charのポインタ、サイズ
	fs.write((char*)&str, sizeof(str));
	//ファイルを閉じる
	fs.close();

	//ファイルを開く in
	std::ifstream fr("data/test.bin");
	STR str2;
	//読み込み
	fr.read((char*)&str2, sizeof(str2));
	fr.close();

	//文字列の書き込み
	const char* s = "ABC\nDEF";
	std::ofstream ff("data/moji.bin",std::ios::binary);
	ff.write(s, strlen(s) + 1);
	ff.close();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (Input::IsKeyOnTrig(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	Pad* pad = FindGameObject<Pad>();
	if (pad->LStickX() >= 0.9f)
	{
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
