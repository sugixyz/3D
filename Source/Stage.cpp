#include "Stage.h"
#include <assert.h>
#include<fstream>
#include"Player.h"
#include"Goblin.h"
#include"StageObject.h"

Stage::Stage()
{
	hModel = MV1LoadModel("data/models/Stage/Stage00.mv1");
	assert(hModel > 0);
	MV1SetupCollInfo(hModel, -1);
	ReadMappingData(0);
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
	bool ret = false;
	//今までで一番startに近いもの
	VECTOR3 tmpPos = end;

	MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(hModel, -1, start, end);
	if (result.HitFlag > 0) {
		tmpPos = result.HitPosition;
		ret = true;
	}
	auto objs = FindGameObjects<StageObject>();
	for (auto obj : objs)
	{
		VECTOR3 hitPos;
		bool res = obj->CollideRay(start, end, &hitPos);
		if (res)
		{
			float tmpFar = VSize(tmpPos - start);
			float hitFar = VSize(hitPos - start);
			if (hitFar <= tmpFar)
			{
				tmpPos = hitPos;
			}
			ret = res;
		}
	}
	if (ret && hit != nullptr)
	{
		*hit = tmpPos;
	}
	return ret;
}

VECTOR3 Stage::CollideSphere(VECTOR3 center, float radius)
{
	//VECTOR3 pushVec;
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(hModel, -1, center, radius);
	for (int i = 0;i < result.HitNum;i++)
	{
		//ToDo: 横向きの壁には当てるけど、上向きの壁には当てない
		//押し返してくる方向ベクトル
		VECTOR3 pushDir = result.Dim[i].HitPosition - center;
		//中心から壁までの距離
		float dist = pushDir.Size();
		VECTOR3 push = pushDir.Normalize() * (radius - dist);
		//ToDo: 複数のポリゴンのうち一番長いのを返すべき
		return push;
	}
	return VECTOR3();
}

void Stage::ReadMappingData(int stageNo)
{
	struct Header {
		char chunk[4]; // "MAPD"
		int CharaInfoNum; // キャラデータの数
		int ObjectInfoNum; // オブジェクトデータの数
		int EventInfoNum; // イベントデータの数
		VECTOR PlayerPosition; // プレイヤー座標
		float PlayerAngle; // プレイヤーのY軸回転
		int RenderType; // レンダリング方法
		int ClearCondition; // クリア条件
		int killCharaNum; // 倒すべき敵の数
		int killTargetChara; // 倒すべき敵の番号
	};
	struct CharaInfo {
		int id; // 種類(1:Goblin, 2:Bee, 3:Golem, 4:RedGoblin
		VECTOR position; // 座標
		float angle; // Y軸回転
	};
	struct ObjectInfo {
		int id; // 種類
		VECTOR position; // 座標
		VECTOR rotation; // 回転
		VECTOR scale; // 拡縮
	};
	struct EventInfo {
		int type; // 種類
		VECTOR position; // 中心座標
		VECTOR area; // 範囲
		int bgm; // 曲番号
		int objectNum; // オブジェクトの数
		int object[8]; // オブジェクトの番号
	};

	std::ifstream ifs("data/models/Stage/Stage00.dat",std::ios::binary);
	assert(ifs);
	Header h;
	ifs.read((char*)&h, sizeof(h));
	new Player(h.PlayerPosition, h.PlayerAngle);
	for (int c = 0; c < h.CharaInfoNum;c++)
	{
		CharaInfo chr;
		ifs.read((char*)&chr, sizeof(chr));
		new Goblin(chr.position, chr.angle);
	}
	for (int o = 0;o < h.ObjectInfoNum;o++)
	{
		ObjectInfo obj;
		ifs.read((char*)&obj, sizeof(obj));
		new StageObject(obj.id, obj.position, obj.rotation, obj.scale);
	}
	ifs.close();
}
