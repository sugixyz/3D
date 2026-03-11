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

	void AddGoblinCounter() { GoblinCounter += 1; }
	void OnResult();
private:
	float gTime;
	enum State {
		COUNT,
		START,
		INPLAY,
		RESULT,
		END
	};
	State state;
	int GoblinCounter;
};
