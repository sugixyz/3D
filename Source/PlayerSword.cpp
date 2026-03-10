#include "PlayerSword.h"
#include"assert.h"
#include"Player.h"

PlayerSword::PlayerSword()
{
	hModel = MV1LoadModel("data/models/Character/Weapon/Sword/sword.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));

	//プレイヤーからどれくらいの位置か
	position = VECTOR3(-30.0f, 120.0f, 20.0f);
	rotation = VECTOR3(0, 0, 90 * DegToRad);
	velocity = VECTOR3(0, 0, 0);

	startTimer = 0.0f;
	endTimer = 0.5f;
	rate = 0.0f;
}

PlayerSword::~PlayerSword()
{
}

void PlayerSword::Update()
{
	if (startTimer < endTimer) {
		rate = startTimer / endTimer;
		startTimer += Time::DeltaTime();
		if (startTimer >= endTimer) {
			DestroyMe();
		}
	}
}

void PlayerSword::Draw()
{
	//剣を横に
	MATRIX mRotZ = MGetRotZ(rotation.z);
	//ポジション適応
	MATRIX mTrans = MGetTranslate(position);
	//回転
	MATRIX mSwing = MGetRotY(180 * rate * DegToRad);

	Player* p = FindGameObject<Player>();
	//プレイヤーの向いてる方向に
	MATRIX pRotY = MGetRotY(p->GetRotation().y);
	//プレイヤーの位置へ移動
	MATRIX pTrans = MGetTranslate(p->GetPos());

	//行列の合成
	MATRIX mat;
	mat = mRotZ;
	mat *= mTrans;
	mat *= mSwing;
	mat *= pRotY;
	mat *= pTrans;

	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);
}
