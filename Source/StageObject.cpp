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

VECTOR3 StageObject::CollideSphere(VECTOR3 center, float radius)
{
	VECTOR3 ret = VECTOR3(0, 0, 0);

	//VECTOR3 pushVec;
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(hHitModel, -1, center, radius);
	for (int i = 0;i < result.HitNum;i++)
	{
		//result.Dim[i].Normal
		if (result.Dim[i].Normal.y > cosf(30.0f * DegToRad))continue;
		//押し返してくる方向ベクトル
		VECTOR3 pushDir = center - result.Dim[i].HitPosition;
		//中心から壁までの距離
		float dist = pushDir.Size();
		VECTOR3 push = pushDir.Normalize() * (radius - dist);
		//ToDo: 複数のポリゴンのうち一番長いのを返すべき
		//return push;
		if (push.Size() >= ret.Size())ret = push;
	}
	MV1CollResultPolyDimTerminate(result);
	return ret;
}
