#include "Object3D.h"
#include"Stage.h"
#include"DxLib.h"

Object3D::Object3D()
{
	hModel = -1;
	HP = -1;
	stamina = -1;
}

Object3D::~Object3D()
{
	if (hModel > 0) {
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Object3D::Draw()
{
//	MV1SetRotationXYZ(hModel, rotation);
//	MV1SetPosition(hModel, position);

	MATRIX mRotY = MGetRotY(rotation.y);
	MATRIX mTrans = MGetTranslate(position);
	MATRIX mat = mRotY * mTrans; // 回してから移動
	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);
	if (stamina > -1)DrawStamina();
	DrawHP();
}

void Object3D::Attack()
{
}

void Object3D::TakeDamage(int damage)
{
	HP -= damage;
	if (HP < 0)HP = 0;
}

void Object3D::DrawHP()
{
	//3D座標を2D画面座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(position + VECTOR3(0, 200, 0));

	//画面内に映っているかチェック
	if (screenPos.z >= 0.0f && screenPos.z <= 1.0f) {

		SetUseZBuffer3D(FALSE);

		int w = 100;
		int h= 10;
		int x = (int)screenPos.x - w / 2;
		int y = (int)screenPos.y - h;
		float rate = HP / HP_MAX;
		unsigned int pColor = COL_BLUE;
		unsigned int eColor = COL_RED;

		DrawFillBox(x - 2, y - 2, x + w + 2, y + h + 2, COL_BLACK); // 背景
		//プレイヤーは青
		if(stamina > -1)DrawFillBox(x, y, x + (int)(w * rate), y + h, pColor); // 残量
		//敵は赤
		else DrawFillBox(x, y, x + (int)(w * rate), y + h, eColor); // 残量

		SetUseZBuffer3D(TRUE);

	}
}
