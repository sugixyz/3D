#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::Update()
{
	SetLightAmbColor(GetColorF(0.8f, 0.8f, 0.8f, 0.0f));
	ChangeLightTypeDir(VECTOR3(1.0f, -1.0f, 0.0f));
	SetLightDifColor(GetColorF(1.0, 0.5f, 0.5f, 0.0f));
	SetLightSpcColor(GetColorF(1.0, 0.5f, 0.5f, 0.0f));

	SetFogEnable(TRUE);
	SetFogColor(50, 50, 80);
	SetFogStartEnd(2.0f, 2000.0f);

	ObjectManager::SetShadowLight(VECTOR3(1, -1, 0));
}
