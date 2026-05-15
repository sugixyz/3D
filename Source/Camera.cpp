#include "Camera.h"
#include"Pad.h"

Camera::Camera()
{
    rotation.y = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Update()
{
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
    VECTOR3 camPos = camMat + lookPos + VECTOR3(0,150,0);
    SetCameraPositionAndTarget_UpVecY(camPos, lookPos + VECTOR3(0, 250, 0));
}

VECTOR3 Camera::Forward()
{
    return VECTOR3(0,0,1) * MGetRotY(rotation.y);
}
