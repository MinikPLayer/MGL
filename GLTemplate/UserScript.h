#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "Util.h"

#include "Cube.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Time.h"
using namespace std;
using namespace glm;
class UserScript : public GameObject
{
	Mesh* mesh;
	vec3 startPos;
public:
	void Start()
	{
		cout << "UserScript start" << endl;
		//Cube c;
		//c.SetShader(Shader::GetDefaultShader());
		Cube* c = new Cube();
		c->SetShader(Shader::GetDefaultShader());

		mesh = AddComponent<Cube>(c); //(Mesh*)AddComponent<Mesh>(c); //Instantiate<Mesh>((Prefab<Mesh>*)c);

		startPos = GetPosition();

		cout << "UserScript Components size: " << (int)GetComponents().size() << endl;
	}

	void Update()
	{
		//mesh->SetPosition(GetPosition() + vec3(0, sin(glfwGetTime()), 0));
		//SetPosition(startPos + vec3(0, sin(glfwGetTime()), 0));
	}

	void Draw()
	{
		cout << "UserScript draw" << endl;
	}
};