#include "Stage.h"
#include <assert.h>

Stage::Stage()
{
	hModel = MV1LoadModel("data/models/Stage/Stage00.mv1");
	assert(hModel > 0);
	MV1SetupCollInfo(hModel, -1);
}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
	MV1DrawModel(hModel);
}

bool Stage::CollideRay(VECTOR3 start, VECTOR3 end, VECTOR3* hit)
{
	MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(hModel, -1, start, end);
	if (result.HitFlag > 0) {
		if (hit != nullptr) {
			*hit = result.HitPosition;
		}
		return true;
	}
	return false;
}
