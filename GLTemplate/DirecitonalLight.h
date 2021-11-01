#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
	float intensity = 1.0f;
};