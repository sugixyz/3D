#include "ObjectManager.h"
#include "GameObject.h"
#include <algorithm>
#include <assert.h>

namespace
{
	std::list<GameObject*>* objects;
	bool needSortDraw;
	GameObject* running;
	int shadowMap;
	//影を描画していればtrue
	bool shadowDrawing = false;
	//ライトの向き
	VECTOR lightDir = VECTOR3(1, -1, 0);
	//シャドウマップのボックス範囲
	VECTOR boxMin = VECTOR3(-1000, -1, -1000);
	VECTOR boxMax = VECTOR3(1000, 1000, 1000);
};

//影表示のための設定
void ShadowInit()
{
	shadowMap = MakeShadowMap(1024, 1024);
}

//影を描画（シャドウマップへ描画）する前にやること
void ShadowPreDraw()
{
	SetShadowMapDrawArea(shadowMap, boxMin, boxMax);
	SetShadowMapLightDirection(shadowMap, lightDir);
	ShadowMap_DrawSetup(shadowMap);
}

//影の描画が終わった時にやること
void ShadowPostDraw()
{
	ShadowMap_DrawEnd();
	SetUseShadowMap(0, shadowMap);
}

//描画が終わった時にやること	
void ShadowEnd()
{
	SetUseShadowMap(0, -1);
}

void ObjectManager::Init()
{
	objects = new std::list<GameObject*>;
	objects->clear();
	needSortDraw = false;
	running = nullptr;
	ShadowInit();
}

void ObjectManager::Update()
{
	for (auto itr = objects->begin(); itr != objects->end(); itr++)
	{
		GameObject* obj = *itr;
		if (obj == nullptr)
			continue;
		if (not obj->DestroyRequested())
		{
			running = obj;
			obj->Update();
			running = nullptr;
		}
		if (obj->DestroyRequested())
		{
			delete obj;
			*itr = nullptr;
		}
	}
	for (auto itr = objects->begin(); itr != objects->end();)
	{
		if (*itr == nullptr)
		{
			itr = objects->erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ObjectManager::Draw()
{
	if (needSortDraw)
	{
		objects->sort([](GameObject* a, GameObject* b) {return a->GetDrawOrder() > b->GetDrawOrder(); });
		needSortDraw = false;
	}
	ShadowPreDraw();
	shadowDrawing = true;
	for (GameObject* obj : *objects)
	{
		if (obj == nullptr || obj->DestroyRequested())
			continue;
		obj->Draw();
	}
	ShadowPostDraw();
	shadowDrawing = false;
	for (GameObject* obj : *objects)
	{
		if (obj == nullptr || obj->DestroyRequested())
			continue;
		obj->Draw();
	}
	ShadowEnd();
}

void ObjectManager::Release()
{
	while (objects->size() > 0)
	{
		auto itr = objects->begin();
		if (*itr != nullptr)
		{
			delete *itr;
		}
		objects->erase(itr);
	}
	objects->clear();
	delete objects;
	objects = nullptr;
}

bool ObjectManager::ShadowDrawing()
{
	return shadowDrawing;
}

void ObjectManager::SetShadowLight(VECTOR dir)
{
	lightDir = dir;
}

void ObjectManager::SetShadowBox(VECTOR min, VECTOR max)
{
	boxMin = min;
	boxMax = max;
}

void ObjectManager::Push(GameObject* obj)
{
	objects->push_back(obj);
	needSortDraw = true;
}

void ObjectManager::SortByDrawOrder()
{
	needSortDraw = true;
}

void ObjectManager::Pop(GameObject* obj)
{
	assert(running != obj);

	for (auto itr = objects->begin(); itr != objects->end(); itr++)
	{
		if (*itr == obj)
		{
			*itr = nullptr;
		}
	}
}

void ObjectManager::DeleteAllGameObject()
{
	assert(running == nullptr);

	for (auto itr = objects->begin(); itr != objects->end(); itr++)
	{
		GameObject* obj = *itr;
		if (not obj->IsDontDestroy())
		{
			delete obj;
			*itr = nullptr;
		}
	}
	for (auto itr = objects->begin(); itr != objects->end();)
	{
		if (*itr == nullptr) {
			itr = objects->erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

const std::list<GameObject*>& ObjectManager::GetAllObject()
{
	return *objects;
}

bool ObjectManager::IsObjectTag(GameObject* obj, std::string tag)
{
	return obj->IsTag(tag);
}
