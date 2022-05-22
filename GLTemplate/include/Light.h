#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "Cube.h"
#include "Color.h"
#include "AssetsLoader.h"
#include "Time.h"
#include "ShadowMap.h"

class Light : public GameObject
{
protected:
	Color color;
	float intensity;

public:
	static vector<Light*> globalLights;
	
	ShadowMap* shadowMap = nullptr;
	GLuint GetShadowMapTextureID();

	Light(bool ortographic, Color color = Color(255, 255, 255, 255), float intensity = 1);
	~Light();

	Color GetColor() { return this->color; }
	virtual void SetColor(Color c);

	Vector3 GetColorVector();

	void SetLocalPosition(Vector3 pos);
};