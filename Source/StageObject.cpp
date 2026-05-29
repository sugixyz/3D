#include "StageObject.h"

StageObject::StageObject(int id, VECTOR3 pos, VECTOR3 rot, VECTOR3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;

	char path[40];
	sprintf_s<40>(path, "data/models/Stage/Stage_Obj%03d.mv1", id);
	hModel = MV1LoadModel(path);

	sprintf_s<40>(path, "data/models/Stage/Stage_Obj%03d_c.mv1", id);
	hHitModel = MV1LoadModel(path);
	MV1SetupCollInfo(hHitModel, -1);

	MATRIX mRotX = MGetRotX(rotation.x);
	MATRIX mRotY = MGetRotY(rotation.y);
	MATRIX mRotZ = MGetRotZ(rotation.z);
	MATRIX mTrans = MGetTranslate(position);
	MATRIX mScale = MGetScale(scale);
	MATRIX mat = mScale * mRotZ * mRotX * mRotY * mTrans; // 回してから移動
	MV1SetMatrix(hHitModel, mat);
}

StageObject::~StageObject()
{
}

bool StageObject::CollideRay(VECTOR3 start, VECTOR3 end, VECTOR3* hit)
{
	MV1RefreshCollInfo(hHitModel, -1);
	MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(hHitModel, -1, start, end);
	if (result.HitFlag > 0) {
		if (hit != nullptr) {
			*hit = result.HitPosition;
		}
		return true;
	}
	return false;
}
