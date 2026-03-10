#include "Goblin.h"
#include <assert.h>
#include "Stage.h"
#include"Player.h"
#include<cmath>
#include "PlayScene.h"
#include"GoblinSword.h"

namespace {
	const float ATTACK_LENGTH = 150.0f;
	const int ATTACK_DAMAGE = 30;
}

Goblin::Goblin()
{
	//SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
	hModel = MV1LoadModel("data/models/Character/Goblin/Goblin.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(GetRand(5000)-2500, 500, GetRand(5000) - 2500);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
	state = State::READY;

	HP_MAX = 80.0f;
	HP = HP_MAX;

	damage = ATTACK_DAMAGE;
}

Goblin::~Goblin()
{
}

void Goblin::Update()
{
	switch (state) {
	case INPLAY:
	{
		UpdateInplay();
		break;
	}
	case STAN:
	{
		UpdateStan();
		break;
	}
	}
}

void Goblin::UpdateInplay()
{
	//プレイヤーとの距離でラジアンをとる
	Player* p = FindGameObject<Player>();
	float dx, dz;
	dx = p->GetPos().x - position.x;
	dz = p->GetPos().z - position.z;
	rotation.y = atan2(dx, dz);

	VECTOR3 moveDir = VECTOR3(0, 0, 0);
	//if (velocity.y >= 0.0f)
	moveDir.z = 6.0f;

	moveDir = moveDir * MGetRotY(rotation.y);
	velocity.x = moveDir.x;
	velocity.z = moveDir.z;
	if (isAttack) { velocity.x = 0; velocity.z = 0; }
	velocity.y -= 0.5f;
	position += velocity;

	//HP管理
	if (isDead())DestroyMe();

	//地面との当たり判定
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	// 足元付近に地面があるかチェック
	if (stage->CollideRay(position + VECTOR3(0, 100, 0), position + VECTOR3(0, -100, 0), &hitPos)) {
		// 「落下中」かつ「地面より下にいる」時だけ着地させる
		if (velocity.y <= 0 && position.y <= hitPos.y) {
			position.y = hitPos.y;
			velocity.y = 0.0f;
		}
	}

	//接触
	float length = VSize(position - p->GetPos());
	if (length < ATTACK_LENGTH && !isAttack) {
		new GoblinSword(position, rotation);
		isAttack = true;
	}
	//0.5秒で攻撃
	if (isAttack) {
		static float timer = 0;
		if (timer < 0.5) {
			timer += Time::DeltaTime();
			if (timer >= 0.5f) {
				Attack();
			}
		}
		//1秒で再度攻撃可能
		else if(timer < 1.0f){
			timer += Time::DeltaTime();
			if (timer >= 1.0f) {
				isAttack = false;
				timer = 0;
			}
		}
	}
}

void Goblin::UpdateStan()
{
	VECTOR3 moveDir = VECTOR3(0, 0, 0);
	//後ろに吹き飛ばす
	static float vel = -20.0f;
	vel *= 0.9;
	moveDir.z = vel;

	moveDir = moveDir * MGetRotY(rotation.y);
	velocity.x = moveDir.x;
	velocity.z = moveDir.z;
	velocity.y -= 0.5f;
	position += velocity;

	//HP管理
	if (isDead())DestroyMe();

	//地面との当たり判定
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	// 足元付近に地面があるかチェック
	if (stage->CollideRay(position + VECTOR3(0, 100, 0), position + VECTOR3(0, -100, 0), &hitPos)) {
		// 「落下中」かつ「地面より下にいる」時だけ着地させる
		if (velocity.y <= 0 && position.y <= hitPos.y) {
			position.y = hitPos.y;
			velocity.y = 0.0f;
		}
	}

	static float timer = 0;
	if (timer < 1.0f)
	{
		timer += Time::DeltaTime();
		if (timer >= 1.0f) {
			state = State::INPLAY;
			timer = 0.0f;
			vel = -20.0f;
		}
	}
}

void Goblin::Attack()
{
	Player* p = FindGameObject<Player>();
	//プレイヤーを取得
	VECTOR3 pPos = p->GetPos();
	float length = VSize(position - pPos);
	//向き
	VECTOR3 forward = VGet(sinf(rotation.y), 0, cosf(rotation.y));
	//敵からプレイヤーへのベクトル
	VECTOR3 toPlayer = VSub(pPos, position);
	toPlayer.y = 0; // 高さは無視
	toPlayer = VNorm(toPlayer);
	//内積
	float dot = VDot(forward, toPlayer);
	// 1.0 なら真ん前、0 なら真横、-1.0 なら真後ろ
	//前方30度（左右15度）
	if (length < ATTACK_LENGTH && dot > 0.866f) {
		p->TakeDamage(damage);
	}
}

void Goblin::KnockBack()
{
	//少し上に飛ばす
	velocity.y += 10.0f;
	state = State::STAN;
}