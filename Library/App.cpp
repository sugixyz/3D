#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Input.h"
#include "Time.h"
#include <DxLib.h>

void AppInit()
{
	Input::Init();
	ObjectManager::Init();
	SceneManager::Init();
	Time::Init();
}

void AppUpdate()
{
	Input::Update();
	SceneManager::Update();
	ObjectManager::Update();
}

void AppDraw()
{
	Time::Refresh();
	ObjectManager::Draw();
	SceneManager::Draw();
}

void AppRelease()
{
	Time::Release();
	SceneManager::Release();
	ObjectManager::Release();
}

bool AppIsExit()
{
	return SceneManager::IsExit();
}