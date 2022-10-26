#pragma once

#include "GameObject.h"
#include "PlaneMesh.h"

// Kinda not working
class Sky : public PlaneMesh
{
public:

	Vector2 sunPos = Vector2(0.5, 0.5);

	Sky();
	void Update();
    void __Draw(shared_ptr<Material> materialOverride) override;
};