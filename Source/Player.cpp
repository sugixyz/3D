#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include<cmath>
#include "PlayScene.h"

Player::Player()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
	hModel = MV1LoadModel("data/models/Character/Player/PC.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
	isJump = false;
    state = READY;
}

Player::~Player()
{
}

void Player::Update()
{
    switch (state) {
    case READY:
    {
        UpdateReady();
        break;
    }
    case INPLAY :
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

void Player::UpdateReady() {
    //地面との当たり判定
    Stage* stage = FindGameObject<Stage>();
    VECTOR3 hitPos;
    // 足元付近に地面があるかチェック
    if (stage->CollideRay(position + VECTOR3(0, 100, 0), position + VECTOR3(0, -100, 0), &hitPos)) {
        // 「落下中」かつ「地面より下にいる」時だけ着地させる
        if (velocity.y <= 0 && position.y <= hitPos.y) {
            position.y = hitPos.y;
        }
    }

    // カメラ更新
    VECTOR3 camPos;
    //Cキーでバックビュー
    if (CheckHitKey(KEY_INPUT_C))camPos = VECTOR3(0, 350, 600) * MGetRotY(rotation.y) + position;
    else camPos = VECTOR3(0, 350, -600) * MGetRotY(rotation.y) + position;
    SetCameraPositionAndTarget_UpVecY(camPos, position + VECTOR3(0, 250, 0));
}

void Player::UpdateInplay()
{
    if (CheckHitKey(KEY_INPUT_D)) rotation.y += 3.0f * DegToRad;
    if (CheckHitKey(KEY_INPUT_A)) rotation.y -= 3.0f * DegToRad;

    VECTOR3 moveDir = VECTOR3(0, 0, 0);
    if (CheckHitKey(KEY_INPUT_W)) {
        moveDir.z = 10.0f;
        if (CheckHitKey(KEY_INPUT_LSHIFT)) moveDir.z *= 2;
    }
    // プレイヤーの向きに合わせて回転させる
    moveDir = moveDir * MGetRotY(rotation.y);

    //速度の更新
    velocity.x = moveDir.x;
    velocity.z = moveDir.z;

    velocity.y -= 0.5f;

    //ジャンプ入力 (接地中のみ)
    if (CheckHitKey(KEY_INPUT_SPACE) && !isJump) {
        velocity.y = 15.0f;
        isJump = true;
    }

    //座標の更新 (ここで一気に移動)
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
            isJump = false;
        }
    }
    else {
        isJump = true; // 足元に地面がなければ空中
    }

    // カメラ更新
    VECTOR3 camPos;
    //Cキーでバックビュー
    if (CheckHitKey(KEY_INPUT_C))camPos = VECTOR3(0, 350, 600) * MGetRotY(rotation.y) + position;
    else camPos = VECTOR3(0, 350, -600) * MGetRotY(rotation.y) + position;
    SetCameraPositionAndTarget_UpVecY(camPos, position + VECTOR3(0, 250, 0));
}

void Player::UpdateOver()
{
}
