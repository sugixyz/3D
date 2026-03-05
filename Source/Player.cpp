#include "Player.h"
#include <assert.h>
#include "Stage.h"

Player::Player()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
	hModel = MV1LoadModel("data/models/Character/Player/PC.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
}

Player::~Player()
{
}

void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		rotation.y += 3.0f * DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		rotation.y -= 3.0f * DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_W)) {
//		position.x += sinf(rotation.y) * 5.0f;
//		position.z += cosf(rotation.y) * 5.0f;

//		VECTOR3 velocity = VECTOR3(sinf(rotation.y), 0, cosf(rotation.y))*5.0f;
		VECTOR3 velocity = VECTOR3(0, 0, 5) * MGetRotY(rotation.y);
		                 //↑回ってない時の移動ベクトル × 回転行列
		position += velocity;
	}
	// 地面との当たり判定
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	if (stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos)) {
		position = hitPos;
	}

	//カメラ位置は、回ってないプレイヤーとの相対位置＊プレイヤーの回転 ＋ プレイヤーの座標
	VECTOR3 camPos = VECTOR3(0, 350, -500) * MGetRotY(rotation.y) + position;
	SetCameraPositionAndTarget_UpVecY(camPos, position+VECTOR3(0, 250, 0));
}
