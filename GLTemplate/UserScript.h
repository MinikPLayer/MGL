#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "Model.h"
#include "Util.h"
#include "Sky.h"

#include "Cube.h"
#include "Light.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <iostream>
#include "Time.h"
#include "Path.h"

using namespace std;
using namespace glm;

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
	shared_ptr<Mesh> lightCube;
	shared_ptr<Model> model;
	Vector3 startPos;
	float counter = 0;

	const int max = 50;
	const int radius = 20;

	bool paused = true;

	shared_ptr<GameObject> spawned;
public:

	shared_ptr<Material> mats[5];

	int cubesSpawned = 0;

	void AddCube(Vector3 pos = Vector3(0, 0, 0))
	{

		//Mesh* mesh = new Mesh();
		//mesh->SetMaterial(Material::GetDefaultMaterial());
		//mesh->SetPosition(Vector3(0, 0, 0));
		//mesh->loadModel("monkey_high.fbx");

		//AddComponent<Mesh>(mesh);

		//Model* model = new Model("cube.fbx", mats[rand() % 2]);
		if (model.get() == nullptr)
			// model = AssetsLoader::LoadModel(Path::Combine({ "Backpack", "backpack.obj" }), mats[0]);//mats[rand() % 2]);
			model = AssetsLoader::LoadModel("rock6.fbx");

		//Model* model = new Model("sphere.fbx", false);
		//model->SetPosition(pos);
		spawned = model->SpawnMesh(mats[4]);
		spawned->SetParent(this);
		spawned->SetPosition(Vector3(0, 10, -10));
		spawned->SetScale(Vector3(0.1, 0.1, 0.1));
		

		/*RotatingCube* component = new RotatingCube(obj.get());
		component->angle = Vector2::Random().Normalized();
		component->distance = (rand() * 151) % 25;
		component->speed = (((rand() * 5311) % 1000) / 750.0) + 0.5;*/

		cubesSpawned++;
	}


	void AddRandomCube()
	{
		const int mult = 100;
		Vector3 dir = Vector3((rand() % (max * mult) ) / mult - max/2, (rand() % (max * mult)) / mult - max / 2, (rand() % (max * mult)) / mult - max / 2);
		dir.Normalize();

		AddCube(dir * ((rand() % (radius * mult)) / mult) /* distance */);
	}

	void Start()
	{

		srand(time(NULL));
		LOG("UserScript start");

		mats[0] = Material::CreatePrefabedMaterial(Material::SILVER);
		mats[1] = Material::CreatePrefabedMaterial(Material::RUBBER);
		mats[2] = Material::CreatePrefabedMaterial(Material::SILVER);
		mats[3] = shared_ptr<Material>(new Material(AssetsLoader::LoadShader("SkyShader.vert", "SkyShader.frag")));
		mats[4] = Material::CreatePrefabedMaterial(Material::SILVER);

		// Load textures
		for (int i = 0; i < 2; i++)
		{
			mats[i].get()->SetMaterialTexture(AssetsLoader::LoadTexture("container2.png"));
			mats[i]->SetMaterialTexture(AssetsLoader::LoadTexture("container2_specular.png", 1), "specular");
		}

		mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("rock6.png"));

		//mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("rock1_specular.png", 1), "specular");

		mats[3]->SetVec2("_resolution_", Vector2(1920, 1080));
		mats[3]->SetVec2("sunPos", Vector2(900, 500));

		mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass.png"));
		mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular.png", 1), "specular");
		mats[2]->SetVec2("texScale", Vector2(0.1, 0.1));
		mats[1]->SetVec2("texScale", Vector2(1, 1));

		/*Cube* sky = new Cube();
		sky->SetMaterial(mats[3]);
		sky->SetScale(Vector3(100, 100, 100));
		sky->SetPosition(Vector3(0, 0, 0));
		AddComponent(sky);*/

		// Add floor
		/*shared_ptr<Model> m = AssetsLoader::LoadModel("cube.fbx");
		auto msh = m->SpawnMesh(mats[2]);
		msh->SetScale(Vector3(100, 2, 100));
		msh->SetPosition(Vector3(0, -3, 0));*/
		// Add Floor
		Mesh* m = new Mesh(mats[2]);
		Vector2 sinMeshSize(100, 100);
		m->SetPosition(-1.0f * Vector3(sinMeshSize.x / 2, 0, sinMeshSize.y / 2));
		m->GenerateMesh(Vector2(50, 50), [](float x, float y) {
			return sin(x) * sin(y);
		});
		AddComponent(m);

		// Add light cube
		LightCube* cube = new LightCube();
		cube->SetPosition(Vector3(2, 2, 2));
		lightCube = AddComponent(cube);



		Material::GetDefaultMaterial()->SetVec3("lightPos", lightCube->GetPosition());

		Vector3 clr = cube->light->color.ToVector3();
		cube->GetMaterial()->SetVec3("lightColor", clr);

		startPos = GetPosition();

		LOG("UserScript Components size: ", (int)GetComponents().size());

		Input::RegisterAxis(Input::Axis("AddCube", Input::Keyboard, Input::NUM_ADD));
		Input::RegisterAxis(Input::Axis("BindCamera", Input::Keyboard, Input::B));
		Input::RegisterAxis(Input::Axis("Pause", Input::Keyboard, Input::P));

		Camera::GetMainCamera()->SetPosition(Vector3(-60, 0, 0));

		Input::RegisterAxis(Input::Axis("LightX", Input::Devices::Keyboard, Input::KBButtons::RIGHT, Input::KBButtons::LEFT));
		Input::RegisterAxis(Input::Axis("LightZ", Input::Devices::Keyboard, Input::KBButtons::UP, Input::KBButtons::DOWN));
		Input::RegisterAxis(Input::Axis("LightY", Input::Devices::Keyboard, Input::KBButtons::PGUP, Input::KBButtons::PGDOWN));

		Input::RegisterAxis(Input::Axis("RotationX", Input::Devices::Keyboard, Input::KBButtons::RIGHT_BRACKET, Input::KBButtons::LEFT_BRACKET));
		Input::RegisterAxis(Input::Axis("RotationY", Input::Devices::Keyboard, Input::KBButtons::APOSTROPHE, Input::KBButtons::SEMICOLON));
		Input::RegisterAxis(Input::Axis("RotationZ", Input::Devices::Keyboard, Input::KBButtons::SLASH, Input::KBButtons::PERIOD));
	}
	
	void Update()
	{
		LightCube* lCube = (LightCube*)lightCube.get();
		if (Input::GetKey(Input::Devices::Keyboard, Input::R))
			lCube->light->color = Color::Red;
		if (Input::GetKey(Input::Devices::Keyboard, Input::G))
			lCube->light->color = Color::Green;
		if (Input::GetKey(Input::Devices::Keyboard, Input::B))
			lCube->light->color = Color::Blue;

		const float lightMoveSpeed = 10;
		Vector3 move(0, 0, 0);
		move.x += Input::GetAxis("LightX") * Time::deltaTime * lightMoveSpeed;
		move.y += Input::GetAxis("LightY") * Time::deltaTime * lightMoveSpeed;
		move.z += -Input::GetAxis("LightZ") * Time::deltaTime * lightMoveSpeed;

		lCube->Move(move);


		Color color;
		color.r = (sin(Time::elapsedTime * 0.6) + 1 ) * 255 / 2.f;
		color.g = (sin(Time::elapsedTime * 0.8) + 1 ) * 255 / 2.f;
		color.b = (sin(Time::elapsedTime * 1.1) + 1 ) * 255 / 2.f;
		Vector3 lColor = color.ToVector3();

		lCube->GetMaterial()->SetVec3("lightColor", lColor);

		for (int i = 2; i < 2; i++)
		{
			mats[i]->SetVec3("light.specular", lColor);
			mats[i]->SetVec3("light.diffuse", lColor * 0.5);
			mats[i]->SetVec3("light.ambient", lColor * 0.1);
			mats[i]->SetVec3("light.strength", 1);
			mats[i]->SetVec3("lightPos", lightCube->GetPosition());
		}

		if (Input::GetButtonDown("Pause"))
		{
			paused = !paused;
			LOG("Paused: ", paused);
		}

		if (!paused)
		{
			counter += Time::deltaTime * (cubesSpawned / 10.0);
		}
		else
		{
			if (Input::GetButtonDown("AddCube"))
				AddRandomCube();
		}
		if (counter >= 1)
		{
			for (int i = 0; counter >= 1; i++)
			{
				AddRandomCube();
				counter--;
			}

			LOG("Cubes spawned: ", cubesSpawned);
		}

		if (Input::GetButtonDown("BindCamera"))
		{
			LOG("Bind camera");
			GameObject* parent = Camera::GetMainCamera()->GetParent();
			if (parent == nullptr)
				Camera::GetMainCamera()->SetParent(mesh.get());
			else
				Camera::GetMainCamera()->SetParent(nullptr);
		}

		float val = sin(Time::elapsedTime);
		if (Input::GetKey(Input::Keyboard, Input::KBButtons::C))
		{
			LOG("Val: ", val);
		}
		//spawned->SetScale(Vector3(val, val, val));
		//lightCube->SetScale(Vector3(val, val, val));
		//this->SetScale(Vector3(val, val, val));
		//this->SetRotation(Vector3(val, val, val));
		//spawned->Rotate(Vector3(Input::GetAxis("RotationX"), Input::GetAxis("RotationY"), Input::GetAxis("RotationZ")) * Time::deltaTime);
		//mats[4]->SetVec2("texScale", Vector2(sin(Time::elapsedTime), cos(Time::elapsedTime)));
		//spawned->SetScale(Vector3(0.00001f, 0.00001f, 0.00001f));
	}
};