#pragma once
#include"../Library/SceneBase.h"

class ModelViewer : public SceneBase
{
public:
	ModelViewer();
	~ModelViewer();
	void Draw() override;
private:
	int hModel;
	MATRIX mat;
};