#include "Light.h"


shared_ptr<Material> LightCube::lightMat = nullptr;

GLuint Light::GetShadowMapTextureID()
{
	return shadowMap->GetDepthMapID();
}

Light::Light(Color color, float intensity)
{
	this->color = color;
	this->intensity = intensity;

	const int size = 1024;
	this->shadowMap = new ShadowMap(size, size);

	this->shadowMap->posFunc = [this] {
		return GetPosition();
	};
}

Light::~Light()
{
	if (this->shadowMap != nullptr)
		delete this->shadowMap;
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
}
