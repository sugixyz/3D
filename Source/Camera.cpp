#include "Camera.h"
#include"Pad.h"

Camera::Camera()
{
    rotation.y = 0.0f;
    fov = 60.0f * DegToRad;
    delayTime = 1.0f;
    delayElapseTime = delayTime;
    shackTime = 1.0f;
    shackElapseTime = shackTime;
}

Camera::~Camera()
{
}

void Camera::Update()
{
    delayElapseTime += Time::DeltaTime();
    shackElapseTime += Time::DeltaTime();

    if (CheckHitKey(KEY_INPUT_C))
    {
        fov = 45.0f * DegToRad;
    }
    else
    {
        fov = 60.0f * DegToRad;
    }
    SetupCamera_Perspective(fov);
    Pad* pad = FindGameObject<Pad>();

    float sX = pad->RStickX();
    rotation.y += 2.0f * DegToRad * sX;

    float sY = pad->RStickY();
    rotation.x += 1.0f * DegToRad * sY;
    if (rotation.x > 89 * DegToRad)rotation.x = 89 * DegToRad;
    if (rotation.x < -30 * DegToRad)rotation.x = -30 * DegToRad;
}

void Camera::Draw()
{
    VECTOR3 camMat = VECTOR3(0, 0, -500) * MGetRotX(rotation.x) * MGetRotY(rotation.y);
    VECTOR3 target = camMat + lookPos + VECTOR3(0, 150, 0);
    if (shackElapseTime < shackTime / 2)
    {
        float shakeDown = position.y - 20.0f;
        float rate = shackElapseTime / shackTime;
        position = target;
        position.y = (shakeDown - position.y) * rate + position.y;
    }
    else if (shackElapseTime < shackTime)
    {
        float shakeDown = position.y + 20.0f;
        float rate = shackElapseTime / shackTime;
        position = target;
        position.y = (shakeDown - position.y) * rate + position.y;
    }
    else if (delayElapseTime < delayTime)
    {
        float rate = delayElapseTime / delayTime;
        position = (target - position) * rate + position;
    }
    else
    {
        position = target;
    }
    SetCameraPositionAndTarget_UpVecY(position, lookPos + VECTOR3(0, 250, 0));

    VECTOR3 min = position - VECTOR3(-1000, -1, -1000);
    VECTOR3 max = position - VECTOR3(1000, 1000, 1000);
    ObjectManager::SetShadowBox(min, max);
}

VECTOR3 Camera::Forward()
{
    return VECTOR3(0,0,1) * MGetRotY(rotation.y);
}

void Camera::Delay(float time)
{
    delayTime = time;
    delayElapseTime = 0.0f;
}

void Camera::Shake()
{
    shackTime = 0.05;
    shackElapseTime = 0.0f;
}
