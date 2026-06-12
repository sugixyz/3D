#include "Goblin.h"
#include <assert.h>
#include "Stage.h"
#include"Player.h"
#include<cmath>
#include "PlayScene.h"

namespace {
	const float ATTACK_LENGTH = 150.0f;
	const int ATTACK_DAMAGE = 30;

	const VECTOR3 TOP_POS = VECTOR3(0, 120, 0);
	const VECTOR3 BOTTOM_POS = VECTOR3(0, 0, 0);
}

Goblin::Goblin(VECTOR pos, float rotY)
{
	//SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 350, -500), VECTOR3(0, 250, 0));
	hModel = MV1LoadModel("data/models/Character/Goblin/Goblin.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = pos;
	rotation = VECTOR3(0, 0, 0);
	rotation.y = rotY;
	velocity = VECTOR3(0, 0, 0);
	state = State::READY;

	HP_MAX = 80.0f;
	HP = HP_MAX;

	damage = ATTACK_DAMAGE;

	//アニメーター
	animator = new Animator(hModel);
	//アニメーションの追加　番号　名前　ループするか
	animator->AddFile(NEUTRAL, "data/models/Character/Goblin/Anim_Neutral.mv1", true);
	animator->AddFile(RUN, "data/models/Character/Goblin/Anim_Run.mv1", true);
	animator->AddFile(KNOCKBACK, "data/models/Character/Goblin/Anim_Damage.mv1", false);
	animState = AnimState::NEUTRAL;
	animator->Play((int)animState);

	//当たり判定用のカプセルの初期化
	capsule = new Capsule(position + TOP_POS, position + BOTTOM_POS, 50.0f);

	playState = sNormal;
	//playState = sRun;
}

Goblin::~Goblin()
{
	delete animator;
	delete capsule;
}

void Goblin::Update()
{
	switch (state) {
	case INPLAY:
		UpdateInplay();
		break;
	}
}

void Goblin::Draw()
{
	Object3D::Draw();

	//カプセルの表示
	DrawCapsule3D(capsule->top, capsule->bottom, capsule->radius,
		10, COL_RED,COL_RED, FALSE);
}

void Goblin::UpdateInplay()
{
	animator->Update();
	capsule->top = position + TOP_POS;
	capsule->bottom = position + BOTTOM_POS;
	switch (playState) {
	case sNormal:
		UpdateNormal();
		break;
	case sChase:
		UpdateChase();
		break;
	case sReturn:
		UpdateReturn();
		break;
	case sKnockback:
		UpdateKnock();
		break;
	}

	//共通処理
	//HP管理
	if (isDead()) {
		DestroyMe();
		GetScene<PlayScene>()->AddGoblinCounter();
	}

	animator->Play((int)animState);

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

void Goblin::UpdateNormal()
{
	//移動速度を0に
	velocity.x = 0;
	velocity.z = 0;

	spawnPoint = position;

	//プレイヤーとの位置関係で追跡モードに
	Player* p = FindGameObject<Player>();
	VECTOR3 toPlayer = p->GetPos() - position;
	//近距離だったら（1ｍ）
	if (toPlayer.Size() < 100.0f)
	{
		playState = PlayState::sChase;
		animState = AnimState::RUN;
	}
	//視野判定、半径10ｍ、視野角60度
	else if (toPlayer.Size() < 1000.0f)
	{
		//ゴブリンの正面のベクトル
		VECTOR3 a = VECTOR3(0, 0, 1) * MGetRotY(rotation.y);
		//プレイヤーへの正規化ベクトル	
		VECTOR3 b = toPlayer.Normalize();
		//内積
		float dot = a.Dot(b);
		if (dot > cos(80 * DegToRad)) {
			playState = PlayState::sChase;
			animState = AnimState::RUN;
		}
	}
}

void Goblin::UpdateChase()
{
	Player* p = FindGameObject<Player>();
	VECTOR3 toPlayer = p->GetPos() - position;
	position += VECTOR3(0, 0, 1) * MGetRotY(rotation.y) * 6.0f;
	//ゴブリンの右のベクトル
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(rotation.y);
	//内積
	float dot = right.Dot(toPlayer);
	//右にいれば、角度を増やす
	if (dot > 0)
	{
		rotation.y += 2.0f * DegToRad;
	}
	//左にいれば、角度を減らす
	else if (dot < 0)
	{
		rotation.y -= 2.0f * DegToRad;
	}

	//スポーンポイントから一定距離離れたら引き返す
	VECTOR3 toSpawn = spawnPoint - position;
	float dist = toSpawn.Size();
	//10m以上だったら引き返す
	if (dist >= 1000.0f)
	{
		playState = PlayState::sReturn;
		animState = AnimState::RUN;
	}
}

void Goblin::UpdateReturn()
{
	VECTOR3 toSpawn = spawnPoint - position;
	position += VECTOR3(0, 0, 1) * MGetRotY(rotation.y) * 3.0f;
	//ゴブリンの右のベクトル
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(rotation.y);
	//内積
	float dot = right.Dot(toSpawn);
	//右にいれば、角度を増やす
	if (dot > 0)
	{
		rotation.y += 2.0f * DegToRad;
	}
	//左にいれば、角度を減らす
	else if (dot < 0)
	{
		rotation.y -= 2.0f * DegToRad;
	}

	//スポーンポイントについたら
	if (toSpawn.Size() <  100.0f)
	{
		playState = PlayState::sNormal;
		animState = AnimState::NEUTRAL;
	}
}

void Goblin::UpdateKnock()
{
	float frame = animator->GetCurrentFrame();

	if (frame < 7.0f)
	{
		VECTOR3 target = position -Forward() * 30.0f;
		Player* p = FindGameObject<Player>();
		VECTOR3 v = p->GetPos() - position;
		position = (target - position) * (frame / 7.0f) + position;
	}

	if (animator->IsFinish())
	{
		playState = PlayState::sNormal;
		animState = AnimState::NEUTRAL;
	}
}

void Goblin::KnockBack()
{
	if (playState == PlayState::sKnockback)return;
	TakeDamage(25);
	//少し上に飛ばす
	velocity.y += 10.0f;
	Player* p = FindGameObject<Player>();
	VECTOR3 toPlayer = p->GetPos() - position;
	toPlayer = toPlayer.Normalize();
	rotation.y = atan2(toPlayer.x, toPlayer.z);

	playState = PlayState::sKnockback;
	animState = AnimState::KNOCKBACK;
}

bool Goblin::AttackHit(VECTOR3 pos1, VECTOR3 pos2)
{
	//線分pos1-pos2と、capsuleの線分がcapsuleの半径より小さいか
	float lenght = Segment_Segment_MinLength(pos1, pos2, capsule->top, capsule->bottom);
	if (lenght <= capsule->radius)
	{
		KnockBack();
		return true;
	}
	return false;
}
