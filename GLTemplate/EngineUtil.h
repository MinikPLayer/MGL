#pragma once

#include "GameObject.h"
#include "Material.h"
class Camera;

void RunEvent(void (GameObject::* callback)());
void RenderScene(Camera* cam = nullptr, shared_ptr<Material> matOverride = nullptr);