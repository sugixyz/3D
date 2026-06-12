#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include<cmath>
#include "PlayScene.h"
#include"Goblin.h"
#include"Camera.h"
#include"Pad.h"

namespace {
    const float STAMINA_MAX = 10.0f;
    const float ATTACK_LENGTH = 200;
    const int ATTACK_DAMAGE = 50;
}

Player::Player(VECTOR pos, float rotY)
{
    SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
    hModel = MV1LoadModel("data/models/Character/Player/PC.mv1");
    assert(hModel > 0);
    int root = MV1SearchFrame(hModel, "root");
    MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
    position = pos;
    rotation = VECTOR3(0, rotY, 0);
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
    animator->AddFile(NEUTRAL, "data/models/Character/Player/Anim_Neutral.mv1", true);
    animator->AddFile(RUN, "data/models/Character/Player/Anim_Run.mv1", true);
    animator->AddFile(ATTACK, "data/models/Character/Player/Anim_Attack1.mv1", false);
    animator->AddFile(ATTACK2, "data/models/Character/Player/Anim_Attack2.mv1", false);
    animator->AddFile(ATTACK3, "data/models/Character/Player/Anim_Attack3.mv1", false);
    animator->AddFile(JUMP_IN, "data/models/Character/Player/Anim_Jump_In.mv1", false);
    animator->AddFile(JUMP_LOOP, "data/models/Character/Player/Anim_Jump_Loop.mv1", false);
    animator->AddFile(JUMP_OUT, "data/models/Character/Player/Anim_Jump_Out.mv1", false);
    animState = AnimState::NEUTRAL;
    animator->Play((int)animState);

    playState = sNormal;
    stateInst = new PlayerNormal(this);

    hWeapon = MV1LoadModel("data/models/Character/Weapon/Sabel/Sabel.mv1");
    assert(hWeapon > 0);
}

Player::~Player()
{
    delete animator;
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

    DrawSphere3D(position + VECTOR3(0, 60, 0), 50, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), 0);
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
    Camera* cam = FindGameObject<Camera>();
    cam->SetPlayerPosition(position);
}

void Player::UpdateInplay()
{
    //アニメーションの再生
    animator->Update();

    stateInst->Update();

    //HP管理
    if (isDead())GetScene<PlayScene>()->OnResult();

    // カメラ更新
    Camera* cam = FindGameObject<Camera>();
    cam->SetPlayerPosition(position);

    //アニメーションの適応
    animator->Play((int)animState);

    Stage* stage = FindGameObject<Stage>();

    VECTOR3 pushVec = stage->CollideSphere(position + VECTOR3(0, 60, 0), 50);
    position += pushVec;

    //地面との当たり判定
    VECTOR3 hitPos;
    // 足元付近に地面があるかチェック
    if (stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos)) {
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
        case PlayState::sJump:
            stateInst = new PlayerJump(this);
            break;
        }
        playState = st;
    }
}

bool Player::Attack()
{
    auto gobs = FindGameObjects<Goblin>();
    for (auto g : gobs)
    {
        //サーベルの座標
        int wp = MV1SearchFrame(hModel, "wp");
        MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
        //手の場所
        VECTOR3 p1 = VECTOR3(0, 0, 0) * m;
        //サーベルの先
        VECTOR3 p2 = VECTOR3(0, -100, 0) * m;

        g->AttackHit(p1,p2);
    }
    return false;
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
    Pad* pad = FindGameObject<Pad>();
    float sX = pad->LStickX();
    float sY = pad->LStickY();
    Camera* cam = FindGameObject<Camera>();
    float speed = 8.0f;
    VECTOR3 vel = { 0,0,0 };

    //ダッシュ
    if (sY >= 0.9f && !pl->isExhausted) {
        speed *= 1.5f;
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
        speed *= 0.8;
        if (pl->stamina > (STAMINA_MAX / 2)) {
            pl->isExhausted = false;
        }
    }

    if (sX != 0.0f || sY != 0.0f) {
        float angle = atan2f(sX, sY);
        vel = cam->Forward() * MGetRotY(angle) * speed;
        VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(pl->rotation.y);

        VECTOR3 velNorm = vel.Normalize();
        float dot = velNorm.Dot(forward);
        if (dot >= cos(30 * DegToRad))
        {
            pl->rotation.y = atan2(vel.x,vel.z);
        }
        else
        {
            VECTOR3 right = VECTOR3(1, 0, 0);
            vel = VECTOR3(0, 0, 0);
            float dot = velNorm.Dot(right);
            if (dot > 0)
            {
                pl->rotation.y += 30.0f * DegToRad;
            }
            else
            {
                pl->rotation.y -= 30.0f * DegToRad;
            }
        }

        pl->animState = Player::AnimState::RUN;
    }
    else
    {
        pl->animState = Player::AnimState::NEUTRAL;
    }

    pl->velocity.x = vel.x;
    pl->velocity.z = vel.z;
    pl->velocity.y -= 0.5f;
    pl->position += pl->velocity;

    //ジャンプ入力 (接地中のみ)
    if (pad->Pushed(Pad::JUMP) && !pl->isJump) {
        pl->ChangPlayState(Player::PlayState::sJump);
        return;
    }

    //座標の更新 (ここで一気に移動)
    pl->position += pl->velocity;

    //攻撃
    //キーを押すと即攻撃
    //if (pad->Pushed(Pad::ATTACK)) 
    if(pad->PushTrigger(Pad::ATTACK))
    {
        pl->ChangPlayState(Player::PlayState::sAttack1);
    }
}

PlayerAttack1::PlayerAttack1(Player* parent)
    :PlayerStateBase(parent)
{
    pl->animState = Player::AnimState::ATTACK;
}

PlayerAttack1::~PlayerAttack1()
{
}

void PlayerAttack1::Update()
{
    Pad* pad = FindGameObject<Pad>();

    float frame = pl->animator->GetCurrentFrame();
    if (frame >= 3.5f && frame <= 8.5f)
    {
        pl->Attack();
    }
    if (pl->animator->GetCurrentFrame() >= 8.5f)
    {
        //if (pad->Pushed(Pad::ATTACK))
        if (pad->PushTrigger(Pad::ATTACK))
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
    pl->animState = Player::AnimState::ATTACK2;
}

PlayerAttack2::~PlayerAttack2()
{
}

void PlayerAttack2::Update()
{
    Pad* pad = FindGameObject<Pad>();
    float frame = pl->animator->GetCurrentFrame();
    if (frame >= 3.5f && frame <= 8.5f)
    {
        pl->Attack();
    }
    if (pl->animator->GetCurrentFrame() >= 8.5f)
    {
        //if (pad->Pushed(Pad::ATTACK))
        if (pad->PushTrigger(Pad::ATTACK))
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
    pl->animState = Player::AnimState::ATTACK3;
}

PlayerAttack3::~PlayerAttack3()
{
}

void PlayerAttack3::Update()
{
    float frame = pl->animator->GetCurrentFrame();
    if (frame >= 3.5f && frame <= 8.5f)
    {
        pl->Attack();
    }
    if (pl->animator->IsFinish())
    {
        pl->ChangPlayState(Player::sNormal);
    }
}

PlayerJump::PlayerJump(Player* parent)
    :PlayerStateBase(parent)
{
    pl->velocity.y = 15.0f;
    pl->animState = Player::AnimState::JUMP_IN;
    pl->isJump = true;
}

PlayerJump::~PlayerJump()
{
}

void PlayerJump::Update()
{
    pl->velocity.y -= 0.5f;
    //座標の更新 (ここで一気に移動)
    pl->position += pl->velocity;

    if (pl->animState == Player::AnimState::JUMP_IN && pl->animator->IsFinish())
    {
        pl->animState = Player::AnimState::JUMP_LOOP;
    }

    if (!pl->IsJump() && pl->animator->IsFinish())
    {
        pl->ChangPlayState(Player::PlayState::sNormal);
    }
    else if (!pl->IsJump())
    {
        pl->animState = Player::AnimState::JUMP_OUT;
    }
}
