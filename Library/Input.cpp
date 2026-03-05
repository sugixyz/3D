#include "Input.h"
#include <assert.h>

namespace {
    char prevKey[256];
    char currentKey[256];
    int prevMouse;
    int currentMouse;
    POINT mousePoint;
};

void Input::Init()
{
    memset(prevKey, 1, 256);
    memset(currentKey, 1, 256);
    prevMouse = -1;
    currentMouse = 0;
    mousePoint.x = -1;
    mousePoint.y = -1;
}

void Input::Update()
{
    memcpy(prevKey, currentKey, 256);
    GetHitKeyStateAll(currentKey);
    prevMouse = currentMouse;
    currentMouse = GetMouseInput();
    int x, y;
    GetMousePoint(&x, &y);
    mousePoint.x = x;
    mousePoint.y = y;
}

bool Input::IsKeyPress(int key)
{
    assert(key>=0 && key<256);
    return currentKey[key] != 0;
}

bool Input::IsKeyOnTrig(int key)
{
    assert(key >= 0 && key < 256);
    return currentKey[key] != 0 && prevKey[key] == 0;
}

bool Input::IsMousePress(int key)
{
    return (currentMouse & key) != 0;
}

bool Input::IsMouseOnTrig(int key)
{
    return (currentMouse & ~prevMouse & key) != 0;
}

const POINT& Input::MousePosition()
{
    return mousePoint;
}