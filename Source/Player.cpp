#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include<cmath>
#include "PlayScene.h"
#include"Goblin.h"
#include"PlayerSword.h"

namespace {
    const float STAMINA_MAX = 10.0f;
    const float ATTACK_LENGTH = 200;
    const int ATTACK_DAMAGE = 50;
}

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

    stamina = STAMINA_MAX;
    isExhausted = false;

    HP_MAX = 100.0f;
    HP = HP_MAX;

    damage = ATTACK_DAMAGE;
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
    }
    else if (CheckHitKey(KEY_INPUT_S)) {
        moveDir.z = -10.0f;
    }
    //ダッシュ
    if (CheckHitKey(KEY_INPUT_LSHIFT) && !isExhausted) {
        moveDir.z *= 1.5f;
        //５０秒でスタミナ切れ
        stamina -= Time::DeltaTime() * 2;
        if (stamina <= 0) {
            stamina = 0.0;
            isExhausted = true;
        }
    }
    else {
        //スタミナ回復 100秒で満タン
        stamina += Time::DeltaTime();
        if (stamina > STAMINA_MAX)stamina = STAMINA_MAX;
    }
    //スタミナ切れ
    if (isExhausted) {
        moveDir.z *= 0.8;
        if (stamina > (STAMINA_MAX / 2)) {
            isExhausted = false;
        }
    }

    //HP管理
    if (isDead())DestroyMe();

    //攻撃
    if (CheckHitKey(KEY_INPUT_RETURN) && !isAttack) {
        Attack();
        isAttack = true;
    }
    if (isAttack) {
        static float timer = 0;
        if (timer < 0.8f) {
            timer += Time::DeltaTime();
            if (timer >= 0.8f) {
                isAttack = false;
                timer = 0;
            }
        }
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

void Player::Attack()
{
    //剣の表示 (見た目だけ)
    new PlayerSword();
    //全敵を検索
    auto goblins = FindGameObjects<Goblin>();
    for (Goblin* gb : goblins) {
        //距離
        VECTOR3 gbPos = gb->GetPos();
        float length = VSize(position - gbPos);
        //向き（プレイヤーの前にいるか）
        VECTOR3 forward = VGet(sinf(rotation.y), 0, cosf(rotation.y));
        //プレイヤーから敵へのベクトル
        VECTOR3 toEnemy = VSub(gbPos, position);
        toEnemy.y = 0; // 高さは無視
        toEnemy = VNorm(toEnemy);
        //内積
        float dot = VDot(forward, toEnemy);
        // 1.0 なら真ん前、0 なら真横、-1.0 なら真後ろ
        //前方180度
        if (length < ATTACK_LENGTH && dot > 0.0f) { 
            gb->TakeDamage(damage);
            gb->KnockBack();
        }
    }
}


void Object3D::DrawStamina()
{
    unsigned int color = COL_GREEN;
    if (stamina <= (STAMINA_MAX / 4))color = COL_RED;
    else if (stamina <= (STAMINA_MAX / 2))color = COL_YELLOW;
    //左上にスタミナゲージ表示
    float x, y, rate, w, h;
    x = 10;
    y = 10;
    w = 200;
    h = 20;
    rate = stamina / STAMINA_MAX;
    DrawBox(x, y, x + (w * rate), y + h, color, TRUE);
    DrawBox(x, y, x + w, y + h, COL_WHITE, FALSE);
}
