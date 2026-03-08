#include "Enemy.h"
#include <assert.h>
#include "Stage.h"
#include"Player.h"
#include<cmath>

Enemy::Enemy()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
	hModel = MV1LoadModel("data/models/Character/Goblin/Goblin.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	//プレイヤーとの距離でラジアンをとる
	Player* p = FindGameObject<Player>();
	float dx, dz;
	dx = p->GetPos().x - position.x;
	dz = p->GetPos().z - position.z;
	rotation.y = atan2(dx, dz);

	VECTOR3 moveDir = VECTOR3(0, 0, 0);

	moveDir = moveDir * MGetRotY(rotation.y);
	velocity.x = moveDir.x;
	velocity.z = moveDir.z;
	velocity.y -= 0.5f;
	position += velocity;

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
}
