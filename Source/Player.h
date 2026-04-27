#pragma once
#include "Object3D.h"
#include"Animator.h"

//プレイヤーというクラスがあることを明示する
class Player;
class PlayerStateBase
{
public:
	PlayerStateBase(Player* parent) { pl = parent; }
	virtual ~PlayerStateBase() {}
	virtual void Update() {}
protected:
	Player* pl;
};

class PlayerNormal : public PlayerStateBase
{
public:
	PlayerNormal(Player* parent);
	~PlayerNormal();
	void Update() override;
};

class PlayerAttack1 : public PlayerStateBase
{
public:
	PlayerAttack1(Player* parent);
	~PlayerAttack1();
	void Update() override;
};

class PlayerAttack2 : public PlayerStateBase
{
public:
	PlayerAttack2(Player* parent);
	~PlayerAttack2();
	void Update() override;
};

class PlayerAttack3 : public PlayerStateBase
{
public:
	PlayerAttack3(Player* parent);
	~PlayerAttack3();
	void Update() override;
};

class PlayerJump : public PlayerStateBase
{
public:
	PlayerJump(Player* parent);
	~PlayerJump();
	void Update() override;
};

class Player : public Object3D
{
	friend class PlayerNormal;
	friend class PlayerAttack1;
	friend class PlayerAttack2;
	friend class PlayerAttack3;
	friend class PlayerJump;
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;
	void Attack() override;
	bool IsJump() { return isJump; }
	void PlayStart() { state = State::INPLAY; }
private:
	bool isJump;
	enum State {
		READY,
		INPLAY,
		OVER
	};
	State state;
	void UpdateReady();
	void UpdateInplay();
	void UpdateOver();

	//スタミナ切れ
	bool isExhausted;

	//アニメーターコントロール用のインスタンス
	Animator* animator;
	enum AnimState
	{
		NEUTRAL,
		RUN,
		JUMP,
		ATTACK,
		ATTACK2,
		ATTACK3
	};
	AnimState animState;

	//プレイ中の状態
	//プレイヤーの状態管理用enum
	enum PlayState
	{
		//通常状態
		sNormal,
		//攻撃1
		sAttack1,
		//攻撃２
		sAttack2,
		//攻撃3
		sAttack3,
		//ジャンプ状態
		sJump,
	};
	//状態管理用変数
	PlayState playState;
	PlayerStateBase* stateInst;

	//武器用ハンドル
	int hWeapon;
private:
	void ChangPlayState(PlayState st);
};