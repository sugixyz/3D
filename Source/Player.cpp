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

    //アニメーター
    animator = new Animator(hModel);
    //アニメーションの追加　番号　名前　ループするか
    animator->AddFile(0, "data/models/Character/Player/Anim_Neutral.mv1", true);
    animator->AddFile(1, "data/models/Character/Player/Anim_Run.mv1", true);
    animator->AddFile(2, "data/models/Character/Player/Anim_Attack1.mv1", false);
    animator->AddFile(3, "data/models/Character/Player/Anim_Attack2.mv1", false);
    animator->AddFile(4, "data/models/Character/Player/Anim_Attack3.mv1", false);
    animState = AnimState::NEUTRAL;
    animator->Play((int)animState);

    playState = sNormal;
    stateInst = new PlayerNormal(this);

    hWeapon = MV1LoadModel("data/models/Character/Weapon/Sabel/Sabel.mv1");
    assert(hWeapon > 0);
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

void Player::Draw()
{
    Object3D::Draw();

    //サーベルの表示
    int wp = MV1SearchFrame(hModel, "wp");
    MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
    MV1SetMatrix(hWeapon, m);
    MV1DrawModel(hWeapon);

    //手の場所
    VECTOR3 p = VECTOR3(0, 0, 0) * m;
    //サーベルの先
    VECTOR3 s = VECTOR3(0, -100, 0) * m;
    DrawLine3D(s, p, COL_RED);
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
    //アニメーションの再生
    animator->Update();

    stateInst->Update();

    //HP管理
    if (isDead())GetScene<PlayScene>()->OnResult();

    // カメラ更新
    VECTOR3 camPos;
    //Cキーでバックビュー
    if (CheckHitKey(KEY_INPUT_C))camPos = VECTOR3(0, 350, 600) * MGetRotY(rotation.y) + position;
    else camPos = VECTOR3(0, 350, -600) * MGetRotY(rotation.y) + position;
    SetCameraPositionAndTarget_UpVecY(camPos, position + VECTOR3(0, 250, 0));

    //アニメーションの適応
    animator->Play((int)animState);

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
}

void Player::UpdateOver()
{
}

void Player::Attack()
{
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

void Player::ChangPlayState(PlayState st)
{
    if (playState != st)
    {
        if (stateInst != nullptr)
        {
            delete stateInst;
        }
        switch (st)
        {
            case PlayState::sNormal:
            stateInst = new PlayerNormal(this);
            break;
        case PlayState::sAttack1:
            stateInst = new PlayerAttack1(this);
            break;
        case PlayState::sAttack2:
            stateInst = new PlayerAttack2(this);
            break;
        case PlayState::sAttack3:
            stateInst = new PlayerAttack3(this);
            break;
        }
        playState = st;
    }
}

PlayerNormal::PlayerNormal(Player* parent)
    : PlayerStateBase(parent)
{
}

PlayerNormal::~PlayerNormal()
{
}

void PlayerNormal::Update()
{
    if (CheckHitKey(KEY_INPUT_D)) pl->rotation.y += 3.0f * DegToRad;
    if (CheckHitKey(KEY_INPUT_A)) pl->rotation.y -= 3.0f * DegToRad;

    VECTOR3 moveDir = VECTOR3(0, 0, 0);
    if (CheckHitKey(KEY_INPUT_W)) {
        moveDir.z = 10.0f;
        //走るアニメーションに
        pl->animState = Player::AnimState::RUN;
    }
    else if (CheckHitKey(KEY_INPUT_S)) {
        moveDir.z = -10.0f;
        //走るアニメーションに
        pl->animState = Player::AnimState::RUN;
    }
    else
    {
        //ニュートラルに
        pl->animState = Player::AnimState::NEUTRAL;
    }
    //ダッシュ
    if (CheckHitKey(KEY_INPUT_LSHIFT) && !pl->isExhausted) {
        moveDir.z *= 1.5f;
        //５０秒でスタミナ切れ
        pl->stamina -= Time::DeltaTime() * 2;
        if (pl->stamina <= 0) {
            pl->stamina = 0.0;
            pl->isExhausted = true;
        }
    }
    else {
        //スタミナ回復 100秒で満タン
        pl->stamina += Time::DeltaTime();
        if (pl->stamina > STAMINA_MAX)pl->stamina = STAMINA_MAX;
    }
    //スタミナ切れ
    if (pl->isExhausted) {
        moveDir.z *= 0.8;
        if (pl->stamina > (STAMINA_MAX / 2)) {
            pl->isExhausted = false;
        }
    }

    // プレイヤーの向きに合わせて回転させる
    moveDir = moveDir * MGetRotY(pl->rotation.y);

    //速度の更新
    pl->velocity.x = moveDir.x;
    pl->velocity.z = moveDir.z;

    pl->velocity.y -= 0.5f;

    //ジャンプ入力 (接地中のみ)
    if (CheckHitKey(KEY_INPUT_SPACE) && !pl->isJump) {
        pl->velocity.y = 15.0f;
        pl->isJump = true;
    }

    //座標の更新 (ここで一気に移動)
    pl->position += pl->velocity;

    //攻撃
    //キーを押すと即攻撃
    if (CheckHitKey(KEY_INPUT_RETURN)) {
        pl->ChangPlayState(Player::PlayState::sAttack1);
    }
}

PlayerAttack1::PlayerAttack1(Player* parent)
    :PlayerStateBase(parent)
{
    pl->Attack();
    pl->animState = Player::AnimState::ATTACK;
}

PlayerAttack1::~PlayerAttack1()
{
}

void PlayerAttack1::Update()
{
    if (pl->animator->GetCurrentFrame() >= 8.5f)
    {
        if (CheckHitKey(KEY_INPUT_RETURN))
        {
            pl->ChangPlayState(Player::sAttack2);
            return;
        }
    }
    if (pl->animator->IsFinish())
    {
        pl->ChangPlayState(Player::sNormal);
    }
}

PlayerAttack2::PlayerAttack2(Player* parent)
    :PlayerStateBase(parent)
{
    pl->Attack();
    pl->animState = Player::AnimState::ATTACK2;
}

PlayerAttack2::~PlayerAttack2()
{
}

void PlayerAttack2::Update()
{
    if (pl->animator->GetCurrentFrame() >= 8.5f)
    {
        if (CheckHitKey(KEY_INPUT_RETURN))
        {
            pl->ChangPlayState(Player::sAttack3);
            return;
        }
    }
    if (pl->animator->IsFinish())
    {
        pl->ChangPlayState(Player::sNormal);
    }
}

PlayerAttack3::PlayerAttack3(Player* parent)
    :PlayerStateBase(parent)
{
    pl->Attack();
    pl->animState = Player::AnimState::ATTACK3;
}

PlayerAttack3::~PlayerAttack3()
{
}

void PlayerAttack3::Update()
{
    if (pl->animator->IsFinish())
    {
        pl->ChangPlayState(Player::sNormal);
    }
}
