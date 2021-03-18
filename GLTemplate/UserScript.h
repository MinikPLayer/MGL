#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "Model.h"
#include "Util.h"

#include "Cube.h"
#include "Light.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <iostream>
#include "Time.h"
using namespace std;
using namespace glm;

class RotatingCube : public GameObject
{

};

class UserScript : public GameObject
{
	Mesh* mesh;
	Mesh* lightCube;
	Vector3 startPos;
	float counter = 100;

	const int max = 50;
	const int radius = 20;

	bool paused = true;
public:

	int cubesSpawned = 0;

	void AddMonkey(Vector3 pos = Vector3(0, 0, 0))
	{
		//Mesh* mesh = new Mesh();
		//mesh->SetMaterial(Material::GetDefaultMaterial());
		//mesh->SetPosition(Vector3(0, 0, 0));
		//mesh->loadModel("monkey_high.fbx");

		//AddComponent<Mesh>(mesh);

		Model* model = new Model("cube.fbx", false);
		model->SetPosition(pos);
		AddComponent<Model>(model);
	}

	void AddCube(Vector3 pos = Vector3(0,0,0))
	{
		// Debug
		AddMonkey(pos);
		return;

		Cube* c = new Cube();
		//c->SetShader(Shader::GetDefaultShader());
		c->SetMaterial(Material::GetDefaultMaterial());
		c->SetPosition(pos);
		const float div = 500;
		//c->SetPosition(vec3((rand() % 1000) / div, (rand() % 1000) / div, (rand() % 1000) / div));

		mesh = AddComponent<Mesh>(c); //(Mesh*)AddComponent<Mesh>(c); //Instantiate<Mesh>((Prefab<Mesh>*)c);
		/*Mesh* m = new Mesh();
		m->LoadFromObj("monkey.obj");

		m->SetPosition(pos);
		m->SetShader(Shader::GetDefaultShader());

		mesh = AddComponent<Mesh>(m);*/

		/*Mesh* m = new Mesh();
		m->SetMaterial(Material::GetDefaultMaterial());
		//m->SetPosition(GetPosition());
		m->SetPosition(pos);

		//m->LoadFromObj("monkey_high.obj");
		m->LoadFromFBX("monkey_high.fbx");

		AddComponent<Mesh>(m);*/

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
		LOG("UserScript start");
		//AddCube();
		
		//AddCube(); // Add second, static cube

		// Add light cube
		LightCube* cube = new LightCube();
		cube->SetPosition(Vector3(2, 2, 2));
		lightCube = AddComponent<Mesh>(cube);

		Material::GetDefaultMaterial()->SetVec3("lightPos", lightCube->GetPosition());
		Material::GetDefaultMaterial()->SetMaterialTexture("container.jpg");
		
		Vector3 clr = cube->light->color.ToVector3();
		cube->GetMaterial()->SetVec3("lightColor", clr);
		//Material::GetDefaultMaterial()->SetVec3("lightColor", clr); //cube->light->color.ToVector3());

		AddMonkey();

		//SetPosition(Vector3(-3, 0, -3));
		startPos = GetPosition();

		LOG("UserScript Components size: ", (int)GetComponents().size());

		Input::RegisterAxis(Input::Axis("AddCube", Input::Keyboard, Input::NUM_ADD));
		Input::RegisterAxis(Input::Axis("BindCamera", Input::Keyboard, Input::B));
		Input::RegisterAxis(Input::Axis("Pause", Input::Keyboard, Input::P));

		//Camera::GetMainCamera()->SetPosition(Vector3(-20, 0, 0));
		Camera::GetMainCamera()->SetPosition(Vector3(0, 0, -30));

	}
	
	void Update()
	{
		//mesh->SetPosition(GetPosition() + vec3(0, sin(glfwGetTime()), 0));
		//SetPosition(startPos + Vector3(0, sin(Time::elapsedTime), 0));
		//lightCube->SetLocalPosition(Vector3(sin(Time::elapsedTime) * 5, cos(Time::elapsedTime) * 10, 0));

		//Camera::GetMainCamera()->yaw += 45 * Time::deltaTime;
		//Camera::GetMainCamera()->Rotate(Vector3(0, 45 * Time::deltaTime, 0));

		LightCube* lCube = (LightCube*)lightCube;
		if (Input::GetKey(Input::Devices::Keyboard, Input::R))
			lCube->light->color = Color::Red;
		if (Input::GetKey(Input::Devices::Keyboard, Input::G))
			lCube->light->color = Color::Green;
		if (Input::GetKey(Input::Devices::Keyboard, Input::B))
			lCube->light->color = Color::Blue;

		//lCube->light->intensity = (sin(Time::elapsedTime) + 1)/2.f;

		//Vector3 lColor = lCube->light->GetColorVector();
		Color color;
		color.r = (sin(Time::elapsedTime * 0.6) + 1 ) * 255 / 2.f;
		color.g = (sin(Time::elapsedTime * 0.8) + 1 ) * 255 / 2.f;
		color.b = (sin(Time::elapsedTime * 1.1) + 1 ) * 255 / 2.f;
		Vector3 lColor = color.ToVector3();

	

		/*lCube->GetMaterial()->SetVec3("light.specular", lColor);
		lCube->GetMaterial()->SetVec3("light.diffuse", lColor * 0.5);
		lCube->GetMaterial()->SetVec3("light.ambient", lColor * 0.1);*/
		lCube->GetMaterial()->SetVec3("lightColor", lColor);

		//Material::GetDefaultMaterial()->SetVec3("viewPos", Camera::GetMainCamera()->GetPosition());
		//Material::GetDefaultMaterial()->SetVec3("lightColor", lCube->light->GetColorVector());
		Material::GetDefaultMaterial()->SetVec3("light.specular", lColor);
		Material::GetDefaultMaterial()->SetVec3("light.diffuse", lColor * 0.5);
		Material::GetDefaultMaterial()->SetVec3("light.ambient", lColor * 0.1);
		Material::GetDefaultMaterial()->SetVec3("lightPos", lightCube->GetPosition());

		if (Input::GetButtonDown("Pause"))
			paused = !paused;

		if (!paused)
		{
			counter += Time::deltaTime * (cubesSpawned / 10.0);
		}
		else
		{
			if (Input::GetButtonDown("AddCube"))
				AddRandomCube();
		}
		if (counter > 1)
		{
			for (int i = 0; counter >= 1; i++)
			{
				AddRandomCube();
				counter--;
			}

			LOG("Cubes spawned: ", cubesSpawned);

			//counter = 0;
		}

		if (Input::GetButtonDown("BindCamera"))
		{
			LOG("Bind camera");
			GameObject* parent = Camera::GetMainCamera()->GetParent();
			if (parent == nullptr)
				Camera::GetMainCamera()->SetParent(mesh);
			else
				Camera::GetMainCamera()->SetParent(nullptr);
		}

		
	}
};