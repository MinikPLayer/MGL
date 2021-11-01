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
#include "Window.h"
#include "DirecitonalLight.h"

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
	const static int lightCubesCount = 1;
	shared_ptr<Mesh> lightCubes[lightCubesCount];
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

		// Add light cube
		for (int i = 0; i < lightCubesCount; i++) {
			LightCube* cube = new LightCube();
			cube->SetPosition(Vector3(i, i + 1, i));
			lightCubes[i] = AddComponent(cube);
		}


		dirLight = shared_ptr<DirectionalLight>(new DirectionalLight());
		dirLight->SetPosition(Vector3(2.f, 3.f, 3.f));

		Camera::GetMainCamera()->SetPosition(dirLight->GetPosition());
		Camera::GetMainCamera()->LookAt(Vector3(0, 0, 0));

		mats[0] = Material::CreatePrefabedMaterial(Material::SILVER);
		mats[1] = Material::CreatePrefabedMaterial(Material::RUBBER);
		mats[2] = Material::CreatePrefabedMaterial(Material::SILVER);
		mats[3] = Material::CreatePrefabedMaterial(Material::SILVER);
		mats[4] = Material::CreatePrefabedMaterial(Material::SILVER);

		// Load textures
		for (int i = 0; i < 3; i++)
		{
			//mats[i].get()->SetMaterialTexture(AssetsLoader::LoadTexture("container2.png"));
			//mats[i]->SetMaterialTexture(AssetsLoader::LoadTexture("container2_specular.png", 1), "specular");


			mats[i]->SetInt("pointLightsCount", lightCubesCount);
			//mats[i]->SetVec3("dirLight.direction", Vector3(-0.2f, -0.3f, -0.3f));
			mats[i]->SetVec3("dirLight.direction", [this]() {
				return -1 * dirLight->GetPosition();
			});
			mats[i]->SetVec3("dirLight.ambient", Vector3(0.05f, 0.05f, 0.05f));
			mats[i]->SetVec3("dirLight.diffuse", Vector3(0.4f, 0.4f, 0.4f));
			mats[i]->SetVec3("dirLight.specular", Vector3(0.5f, 0.5f, 0.5f));
			mats[i]->SetFloat("dirLight.strength", 1.0f);

			mats[i]->SetVec3("viewPos", []() {
				return Camera::GetMainCamera()->GetPosition();
			});

			for (int j = 0; j < lightCubesCount; j++) {
				string prefix = "pointLights[" + to_string(j) + "].";
				mats[i]->SetFloat(prefix + "constant", 1.0f);
				mats[i]->SetFloat(prefix + "linear", 0.09f);
				mats[i]->SetFloat(prefix + "quadratic", 0.032f);
				mats[i]->SetFloat(prefix + "strength", 1);

				mats[i]->SetVec3(prefix + "specular", [this, j]() {
					LightCube* c = (LightCube*)lightCubes[j].get();
					return c->light->GetColorVector();
				});
				mats[i]->SetVec3(prefix + "diffuse", [this, j]() {
					LightCube* c = (LightCube*)lightCubes[j].get();
					return c->light->GetColorVector() * 0.8;
				});
				mats[i]->SetVec3(prefix + "ambient", [this, j]() {
					LightCube* c = (LightCube*)lightCubes[j].get();
					return c->light->GetColorVector() * 0.5;
				});

				mats[i]->SetVec3(prefix + "position", [this, j]() {
					return lightCubes[j]->GetPosition();
				}); 
			}
		}



		mats[0].get()->SetMaterialTexture(AssetsLoader::LoadTexture("poland.png"));
		mats[0]->SetMaterialTexture(AssetsLoader::LoadTexture("poland_specular.png", 1), "specular");

		mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass.png"));
		mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular.png", 1), "specular");

		mats[3]->SetMaterialTexture(AssetsLoader::LoadTexture("tall_grass.png"));
		mats[3]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular.png", 1), "specular");

		mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("rock6.png"));
		mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("rock1_specular.png", 1), "specular");

		// Add Floor
		floorMesh = shared_ptr<Mesh>(new Mesh(mats[2]));
		Vector2 sinMeshSize(50, 50);
		floorMesh->SetLocalPosition(-1.0f * Vector3(sinMeshSize.x / 2, 0, sinMeshSize.y / 2));
		floorMesh->GenerateMesh(sinMeshSize, [](float x, float y) {
			//return sin(x/2.0f) * sin(y/2.0f);
			return 0.0f;
		}, nullptr, 0.5);
		AddComponent(floorMesh);

		PlaneMesh* plane = new PlaneMesh();
		plane->SetMaterial(mats[3]);
		plane->SetScale(Vector3(5, 5, 5));
		plane->SetPosition(Vector3(5, 10, 5));
		plane->faceCulling = Mesh::FaceCullingModes::Disabled;
		AddComponent(plane);

		startPos = GetPosition();

		LOG("UserScript Components size: ", (int)GetComponents().size());

		Input::RegisterAxis(Input::Axis("AddCube", Input::Keyboard, Input::NUM_ADD));
		Input::RegisterAxis(Input::Axis("BindCamera", Input::Keyboard, Input::B));
		Input::RegisterAxis(Input::Axis("Pause", Input::Keyboard, Input::P));

		Input::RegisterAxis(Input::Axis("LightX", Input::Devices::Keyboard, Input::KBButtons::RIGHT, Input::KBButtons::LEFT));
		Input::RegisterAxis(Input::Axis("LightZ", Input::Devices::Keyboard, Input::KBButtons::UP, Input::KBButtons::DOWN));
		Input::RegisterAxis(Input::Axis("LightY", Input::Devices::Keyboard, Input::KBButtons::PGUP, Input::KBButtons::PGDOWN));

		Input::RegisterAxis(Input::Axis("RotationX", Input::Devices::Keyboard, Input::KBButtons::RIGHT_BRACKET, Input::KBButtons::LEFT_BRACKET));
		Input::RegisterAxis(Input::Axis("RotationY", Input::Devices::Keyboard, Input::KBButtons::APOSTROPHE, Input::KBButtons::SEMICOLON));
		Input::RegisterAxis(Input::Axis("RotationZ", Input::Devices::Keyboard, Input::KBButtons::SLASH, Input::KBButtons::PERIOD));
	}
	
	void Update()
	{
		for (int i = 0; i < lightCubesCount; i++) {
			LightCube* lCube = (LightCube*)lightCubes[i].get();

			const float lightMoveSpeed = 10;
			Vector3 move(0, 0, 0);
			move.x += Input::GetAxis("LightX") * Time::deltaTime * lightMoveSpeed;
			move.y += Input::GetAxis("LightY") * Time::deltaTime * lightMoveSpeed;
			move.z += -Input::GetAxis("LightZ") * Time::deltaTime * lightMoveSpeed;

			lCube->Move(move * (i+1));
		}

		if (Input::GetButtonDown("Pause"))
		{
			paused = !paused;
			LOG("Paused: ", paused);
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

		if (Input::GetKey(Input::Keyboard, Input::KBButtons::LCTRL)) {
			glDisable(GL_FRAMEBUFFER_SRGB);
			Window::GetMainWindow()->SetCursorMode(Window::CursorModes::Enabled);
		}
		else {
			glEnable(GL_FRAMEBUFFER_SRGB);
			Window::GetMainWindow()->SetCursorMode(Window::CursorModes::Disabled);
		}

		if(!paused)
			floorMesh->Rotate(Vector3(0, Time::deltaTime, 0));

		//spawned->SetScale(Vector3(val, val, val));
		//lightCube->SetScale(Vector3(val, val, val));
		//this->SetScale(Vector3(val, val, val));
		//this->SetRotation(Vector3(val, val, val));
		//spawned->Rotate(Vector3(Input::GetAxis("RotationX"), Input::GetAxis("RotationY"), Input::GetAxis("RotationZ")) * Time::deltaTime);
		//mats[4]->SetVec2("texScale", Vector2(sin(Time::elapsedTime), cos(Time::elapsedTime)));
		//spawned->SetScale(Vector3(0.00001f, 0.00001f, 0.00001f));
	}
};