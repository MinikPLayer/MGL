#pragma once

#include "Camera.h"

class FlybackCamera : public Camera
{
public:
	float speed = 20.5f;
	float sensitivity = 0.1f;

	void Start();
	void Update();

	void ProcessMouseMovement(float x, float y, bool constrainPitch = true);
	void ProcessMouseScroll(float offset);
};