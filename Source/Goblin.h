#pragma once
#include "Object3D.h"
#include"Animator.h"

class Capsule
{
public:
	Capsule(VECTOR3 p1, VECTOR3 p2, float rad) {
		top = p1;
		bottom = p2;
		radius = rad;
	}
	VECTOR3 top;
	VECTOR3 bottom;
	float radius;
};

/// <summary>
/// ゴブリンの管理
/// </summary>
class Goblin : public Object3D
{
public:
	Goblin();
	~Goblin();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// ゲームスタートを知らせる
	/// </summary>
	void PlayStart() { state = State::INPLAY; }
	/// <summary>
	/// ノックバックさせる
	/// </summary>
	void KnockBack();
	/// <summary>
	/// プレイヤーから攻撃を受ける
	/// </summary>
	/// <param name="pos1">線分の始点</param>
	/// <param name="pos2">線分の終点</param>
	/// <returns>当たってたらtrue	</returns>
	bool AttackHit(VECTOR3 pos1, VECTOR3 pos2);
private:
	enum State {
		READY,
		INPLAY
	};
	State state;
	void UpdateInplay();

	//アニメーターコントロール用のインスタンス
	Animator* animator;
	enum AnimState
	{
		NEUTRAL,
		RUN,
		WALK,
		KNOCKBACK
	};
	AnimState animState;

	//プレイ中の状態
	//ゴブリンの状態管理用enum
	enum PlayState
	{
		//通常状態
		sNormal,
		//追跡状態
		sChase,
		//元の場所に戻る
		sReturn,
		//ノックバック
		sKnockback
	};
	//状態管理用変数
	PlayState playState;
	void UpdateNormal();
	void UpdateChase();
	void UpdateReturn();
	void UpdateKnock();

	//当たり判定用カプセル
	Capsule* capsule;

	//スポーンポイントを保存
	VECTOR3 spawnPoint;
};