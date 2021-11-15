#include "Light.h"
#include "UserScript.h"

shared_ptr<Material> LightCube::lightMat = nullptr;
vector<Light*> Light::globalLights;

GLuint Light::GetShadowMapTextureID()
{
	return shadowMap->GetDepthMapID();
}

Light::Light(bool ortographic, Color color, float intensity)
{
	this->color = color;
	this->intensity = intensity;

	const int size = 2048;
	this->shadowMap = new ShadowMap(ortographic, size, size);

	this->shadowMap->posFunc = [this] {
		return GetPosition();
	};

	globalLights.push_back(this);
}

Light::~Light()
{
	if (this->shadowMap != nullptr)
		delete this->shadowMap;

	for (int i = 0; i < globalLights.size(); i++)
	{
		if (globalLights[i] == this)
		{
			globalLights.erase(globalLights.begin() + i, globalLights.begin() + i + 1);
			break;
		}
	}
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
