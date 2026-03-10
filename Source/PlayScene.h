#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	void OnClear();
	void OnOver();
private:
	float gTime;
	enum State {
		COUNT,
		START,
		INPLAY,
		CLEAR,
		OVER,
		END
	};
	State state;
};
