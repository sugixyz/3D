#include "Object3D.h"
#include"Stage.h"

Object3D::Object3D()
{
	hModel = -1;
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
	MATRIX mat = mRotY * mTrans; // ‰ñ‚µ‚Ä‚©‚çˆÚ“®
	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);
}
