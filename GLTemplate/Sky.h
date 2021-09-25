#pragma once

#include "GameObject.h"
#include "Cube.h"

// Kinda not working
class Sky : public Cube
{
public:

	Vector2 sunPos = Vector2(0.5, 0.5);

	Sky();
	void Update();
};