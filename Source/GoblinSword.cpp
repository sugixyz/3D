#include "GoblinSword.h"
#include"assert.h"
#include"Goblin.h"

GoblinSword::GoblinSword()
{
}

GoblinSword::GoblinSword(VECTOR3 pos, VECTOR3 rot)
{
	hModel = MV1LoadModel("data/models/Character/Weapon/Sword/sword.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));

	//プレイヤーからどれくらいの位置か
	position = VECTOR3(0, 150.0f, 10.0f);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);

	startTimer = 0.0f;
	endTimer = 1.0f;
	rate = 0.0f;

	gbPosition = pos;
	gbRotation = rot;
}

GoblinSword::~GoblinSword()
{
}

void GoblinSword::Update()
{
	if (startTimer < endTimer) {
		rate = startTimer / endTimer;
		startTimer += Time::DeltaTime();
		if (startTimer >= endTimer) {
			DestroyMe();
		}
	}
}

void GoblinSword::Draw()
{
	//ポジション適応
	MATRIX mTrans = MGetTranslate(position);
	//回転
	MATRIX mSwing = MGetRotX(90 * rate * DegToRad);

	//敵の向いてる方向に
	MATRIX gbRotY = MGetRotY(gbRotation.y);
	//敵の位置へ移動
	MATRIX gbTrans = MGetTranslate(gbPosition);

	//行列の合成
	MATRIX mat;
	mat = mTrans;
	mat *= mSwing;
	mat *= gbRotY;
	mat *= gbTrans;

	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);
}
