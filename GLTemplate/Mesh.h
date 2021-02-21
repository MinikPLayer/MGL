#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "GameObject.h"

using namespace glm;

class Mesh : public GameObject
{
protected:
	float* vertexData = nullptr;
	int vertexDataSize = 0;

	bool initialized = false;
	GLuint VAO = 0;
	GLuint VBO = 0;
	
	Shader* shader;
	GLint shaderModelPosition = 0;

	void UpdateShaderInfo()
	{
		shaderModelPosition = shader->GetUniformLocation("model");
	}

	void InitObjects()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);

		initialized = true;
	}
public:
	//vec3 pos = vec3(0, 0, 0);
	void SetShader(Shader* shader)
	{
		this->shader = shader;
		UpdateShaderInfo();
	}

	/*void CopyValues(GameObject* instance)
	{
		cout << "Copy values" << endl;

		GameObject::CopyValues(instance);

		Mesh* mesh = (Mesh*)instance;

		cout << "Setting cube prefab " << endl;

		CopyFrom(mesh->vertexData, mesh->vertexDataSize);
		SetShader(mesh->shader);
	}*/

	void CopyFrom(float* vertexData, int size)
	{
		if (initialized)
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
			
			initialized = false;
		}

		InitObjects();

		if (this->vertexData != nullptr)
		{
			delete[] this->vertexData;
			this->vertexData = nullptr;
			vertexDataSize = 0;
		}

		this->vertexData = new float[size];
		for (int i = 0; i < size; i++)
		{
			this->vertexData[i] = vertexData[i];
		}
		vertexDataSize = size;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertexDataSize * sizeof(float), vertexData, GL_STATIC_DRAW);

		// Vertex pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Vertex UV
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Vertex color - white
		glVertexAttrib3f(1, 1, 1, 1);
	}

	/*Mesh(Shader* shader, float* vertexData, int size)
	{
		this->shader = shader;
		UpdateShaderInfo();
		InitObjects();
		CopyFrom(vertexData, size);
	}

	Mesh(Shader* shader)
	{
		this->shader = shader;
		UpdateShaderInfo();
		InitObjects(); 	
	}*/

	void Start()
	{
		
		cout << "Mesh start" << endl;
	}

	void Update()
	{
		cout << "Mesh update, vertex data size: " << vertexDataSize << endl;
	}

	void Draw()
	{
		if (vertexData == nullptr)
			return;

		if (!initialized)
			return;

		glBindVertexArray(VAO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, GetPosition());
		shader->SetMat4(shaderModelPosition, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, vertexDataSize / 5);
	}

	~Mesh()
	{
		if (vertexData != nullptr)
		{
			delete[] vertexData;
			vertexData = nullptr;
			vertexDataSize = 0;
		}

	}
};