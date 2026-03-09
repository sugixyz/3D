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
private:
	float gTime;
	enum State {
		READY,
		INPLAY,
		CLEAR,
		OVER,
		END
	};
	State state;
};
