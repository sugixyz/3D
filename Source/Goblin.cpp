#include "Goblin.h"
#include <assert.h>
#include "Stage.h"
#include"Player.h"
#include<cmath>
#include "PlayScene.h"

namespace {
	const float DIST_LENGTH = 40.0f;
}

Goblin::Goblin()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
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
	case OVER:
	{
		UpdateOver();
		break;
	}
	}
}

void Goblin::UpdateReady()
{
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

	//プレイヤーとの当たり判定
	//接触
	float length = VSize(position - p->GetPos());
	if (length < DIST_LENGTH) {

	}
}

void Goblin::UpdateOver()
{
}