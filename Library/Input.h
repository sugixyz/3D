#pragma once
#include <DxLib.h>

 namespace Input {
	void Init();
	void Update();

	// キーボード
	bool IsKeyPress(int key);
	bool IsKeyOnTrig(int key);

	// マウス
	bool IsMousePress(int button);
	bool IsMouseOnTrig(int button);
	const POINT& MousePosition();
 };