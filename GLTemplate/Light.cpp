#include "Light.h"


shared_ptr<Material> LightCube::lightMat = nullptr;

Light::Light(Color color, float intensity)
{
	this->color = color;
	this->intensity = intensity;
}

Vector3 Light::GetColorVector()
{
	return color.ToVector3() * intensity;
}
