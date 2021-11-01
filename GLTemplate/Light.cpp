#include "Light.h"


shared_ptr<Material> LightCube::lightMat = nullptr;

Light::Light(Color color, float intensity)
{
	this->color = color;
	this->intensity = intensity;
}

void Light::SetColor(Color c)
{
	this->color = c;
}

Vector3 Light::GetColorVector()
{
	return color.ToVector3() * intensity;
}

void Light::SetLocalPosition(Vector3 pos)
{
	GameObject::SetLocalPosition(pos);

	this->shadowMap.pos = pos;
}
