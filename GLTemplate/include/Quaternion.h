#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Vector.h"

class Quaternion
{
protected:
	glm::quat quaternion;

public:
	Quaternion FromEuler(Vector3 euler);
	Quaternion FromEuler(float x, float y, float z);

	Vector3 ToEuler();

	glm::quat GetGlVector();

	Quaternion(float x = 0, float y = 0, float z = 0);
	Quaternion(Vector3 rot);
	Quaternion operator+(Quaternion q);
	Quaternion operator-(Quaternion q);
	Quaternion operator+(Vector3 euler);
	Quaternion operator-(Vector3 euler);
};