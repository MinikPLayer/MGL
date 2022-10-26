#pragma once

#include "../GameObject.h"
#include "../Mesh.h"
#include "../Model.h"
#include "../Util.h"
#include "../Sky.h"

#include "../Cube.h"
#include "../Light.h"
#include "../Shader.h"

#include <glm/glm.hpp>
#include <iostream>
#include "../TimeUtils.h"
#include "../Path.h"
#include "../Window.h"
#include "../DirecitonalLight.h"
#include "../PointLight.h"

#include <cmath>
//#define M_PI   3.14159265358979323846264338327950288

using namespace std;
using namespace glm;


class LightCube : public Mesh
{
	static shared_ptr<Material> lightMat;
public:
	shared_ptr<Light> light;

	LightCube()
	{
		light = AddComponent<PointLight>();
		light->SetColor(Color::Blue);

		// Debug
		Mesh::CopyFrom(Cube::cubeVertexData, Cube::cubeIndices);

		if (lightMat == nullptr)
		{
			lightMat = shared_ptr<Material>(new Material(AssetsLoader::LoadShader("VertexLightShader.vert", "FragmentLightShader.frag")));
		}

		SetMaterial(lightMat);
		material->SetVec3("objectColor", Vector3(1.0, 1.0, 1.0));
		material->SetVec3("lightColor", [this]() {
			return light->GetColorVector();
		});
	}

	void Update() override {
		Color color;
		color.r = (sin(Time::elapsedTime * 0.6) + 1) * 255 / 2.f;
		color.g = (sin(Time::elapsedTime * 0.8) + 1) * 255 / 2.f;
		color.b = (sin(Time::elapsedTime * 1.1) + 1) * 255 / 2.f;
		light->SetColor(color);
	}
};


class RotatingCube : public GameObject
{
public:
	float distance;
	float speed = 1;
	Vector2 angle;

	void Update()
	{
		float alfa, beta;
		alfa = angle.x * Time::elapsedTime * speed;
		beta = angle.y * Time::elapsedTime * speed;

		Vector3 newPos = Vector3(
			distance * cos(alfa) * cos(beta),
			distance * cos(alfa) * sin(beta),
			distance * sin(alfa)
		);

		SetLocalPosition(newPos);
	}

	RotatingCube(GameObject* parent)
		:GameObject(parent, false) {}
};

class UserScript : public GameObject
{
	shared_ptr<Mesh> mesh;
	const static int lightCubesCount = 1;
	shared_ptr<Mesh> lightCubes[lightCubesCount + 1];
	shared_ptr<Model> model;
	shared_ptr<Mesh> floorMesh;
	float floorOffset = 0;
	Vector3 startPos;
	float counter = 0;

	const int max = 50;
	const int radius = 20;

	bool paused = true;

	shared_ptr<GameObject> spawned;
	shared_ptr<DirectionalLight> dirLight;
public:

	shared_ptr<Material> mats[5];

	int cubesSpawned = 0;

	shared_ptr<GameObject> AddTeapot(Vector3 pos = Vector3(0, 0, 0));
	shared_ptr<GameObject> AddCube(Vector3 pos = Vector3(0, 0, 0));
	void AddRandomCube();

	void Start();
	void Update();
};