#include "ModelViewer.h"
#include<assert.h>

ModelViewer::ModelViewer()
{
	hModel = MV1LoadModel("data/models/Fighter/Fighter.mv1");
	assert(hModel > 0);

	mat = MGetIdent();
	mat *= MGetRotX(DX_PI_F / 2);
}

ModelViewer::~ModelViewer()
{
}

void ModelViewer::Draw()
{
	mat *= MGetRotZ(0.01f);
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 0, -500), VECTOR3(0, 0, 0));
	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);
}
