#include "Enemy.h"
#include <assert.h>
#include "Stage.h"
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
}
