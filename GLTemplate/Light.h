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

	ShadowMap shadowMap;
public:


	Light(Color color = Color(255, 255, 255, 255), float intensity = 1);

	Color GetColor() { return this->color; }
	virtual void SetColor(Color c);

	Vector3 GetColorVector();

	void SetLocalPosition(Vector3 pos);
};

class LightCube : public Mesh
{
	//static Material* lightMat;
	static shared_ptr<Material> lightMat;
public:
	shared_ptr<Light> light;

	LightCube()
	{
		light = AddComponent<Light>();
		light.get()->SetColor(Color::Blue);

		// Debug
		//Mesh::CopyFrom((float*)Cube::vertCube, Cube::vertCubeSize, (unsigned int*)Cube::indicesCube, Cube::indicesCubeSize);
		//Mesh::CopyFrom(Cube::cubeVerticies, Cube::cubeNormals, Cube::cubeUVs, Cube::cubeIndices);
		Mesh::CopyFrom(Cube::cubeVertexData, Cube::cubeIndices);

		if (lightMat == nullptr)
		{
			//lightMat = Material::GetDefaultMaterial();
			lightMat = shared_ptr<Material>(new Material()); //new Material();
			lightMat->shader = AssetsLoader::LoadShader("VertexLightShader.vert", "FragmentLightShader.frag"); //shared_ptr<Shader>(new Shader("VertexLightShader.vert", "FragmentLightShader.frag"));
		}

		//SetShader(new Shader("VertexLightShader.vert", "FragmentLightShader.frag"));
		SetMaterial(lightMat);
		material.get()->SetVec3("objectColor", Vector3(1.0, 1.0, 1.0));
		material.get()->SetVec3("lightColor", [this]() {
			return light->GetColorVector();
		}); 
	}

	void Update() {
		Color color;
		color.r = (sin(Time::elapsedTime * 0.6) + 1) * 255 / 2.f;
		color.g = (sin(Time::elapsedTime * 0.8) + 1) * 255 / 2.f;
		color.b = (sin(Time::elapsedTime * 1.1) + 1) * 255 / 2.f;
		light->SetColor(color);
	}
};
