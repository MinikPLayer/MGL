#pragma once

//#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Shader.h"
#include "Material.h"
#include "GameObject.h"
#include <string>

using namespace std;
using namespace glm;

struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 UV;

	Vertex(Vector3 pos, Vector3 normal, Vector2 UV)
	{
		this->pos = pos;
		this->normal = normal;
		this->UV = UV;
	}

	Vertex()
	{
		this->pos = Vector3(0, 0, 0);
		this->normal = Vector3(0, 0, 0);
		this->UV = Vector2(0, 0);
	}
};


class Mesh : public GameObject
{
public:

	vector<Vertex> vertexData;
	vector<unsigned int> indices;

	/*float* vertexData = nullptr;
	int vertexDataSize = 0;
	unsigned int* indices = nullptr;
	int indicesSize = 0;*/
	
	
	bool initialized = false;

	static bool updateRenderer;
protected:


	
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;

	//Shader* shader;
	shared_ptr<Material> material;

	enum FaceCullingModes
	{
		Default,
		Front,
		Back,
		FrontAndBack
	};

	FaceCullingModes faceCulling = FaceCullingModes::Default;
	

	void UpdateShaderInfo()
	{
		//shaderModelPosition = shader->GetUniformLocation("model");
	}

	void InitObjects()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		initialized = true;
	}
public:
	//bool LoadFromObj(string filePath);
	//bool LoadFromFBX(string filePath);
	Vector3 color;

	//vec3 pos = vec3(0, 0, 0);
	void SetMaterial (shared_ptr<Material> mat)
	{
		this->material = mat;
		UpdateShaderInfo();
	}

	//Shader* GetShader()
	shared_ptr<Material> GetMaterial()
	{
		return this->material;
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

	/*void CopyFrom_old(float* vertexData, int size, unsigned int* indices, int indSize)
	{
		// Re-initialize
		if (vertexData != this->vertexData)
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
			if (this->indices != nullptr)
			{
				delete[] this->indices;
				this->indices = nullptr;
				indicesSize = 0;
			}

			this->vertexData = new float[size];
			for (int i = 0; i < size; i++)
			{
				this->vertexData[i] = vertexData[i];
			}

			this->indices = new unsigned int[indSize];
			for (int i = 0; i < indSize; i++)
			{
				this->indices[i] = indices[i];
			}
		}
		vertexDataSize = size;
		indicesSize = indSize;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertexDataSize * sizeof(float), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(float), indices, GL_STATIC_DRAW);

		// Vertex pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Vertex UV
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(2);

		// Vertex color - white
		//glVertexAttrib3f(1, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0);
		color = Vector3((rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0);
		glVertexAttrib3f(1, color.x, color.y, color.z);
	}*/

	void CopyFrom(Mesh* mesh)
	{
		CopyFrom(mesh->vertexData, mesh->indices);
		SetMaterial(mesh->GetMaterial());
	}

	void CopyFrom(vector<Vertex> vertexData, vector<unsigned int> indices)
	{
		this->vertexData = vertexData;
		this->indices = indices;

		if (initialized)
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);

			initialized = false;

			InitObjects();
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), &(vertexData[0]), GL_STATIC_DRAW);

		if (indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);
		}

		// Vertex pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//offsetof(Vertex, pos));
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		// Vertex UV
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		glEnableVertexAttribArray(2);

		// Vertex color - white
		//glVertexAttrib3f(1, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0);
		//color = Vector3((rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0);
		color = Vector3(1, 1, 1);
		//glVertexAttrib3f(3, color.x, color.y, color.z);
		glVertexAttrib3f(3, 1, 1, 1);

		//InitObjects();
	}

	void CopyFrom(vector<Vector3> vertex, vector<Vector3> normals, vector<Vector2> uv, vector<unsigned int> indices)
	{
		if (vertex.size() != normals.size() || normals.size() != uv.size())
		{
			LOGE("Mismatch in data size");
			return;
		}

		vector<Vertex> vertexes;
		for (int i = 0; i < vertex.size(); i++)
			vertexes.push_back(Vertex(vertex[i], normals[i], uv[i]));

		CopyFrom(vertexes, indices);
	}

	Vector3 CalculateTriangleNormal(Vertex vtx1, Vertex vtx2, Vertex vtx3)
	{
		Vector3 v1, v2;
		v1 = vtx2.pos - vtx1.pos;
		v2 = vtx3.pos - vtx1.pos;

		return Vector3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.z - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	void GenerateMesh(Vector2 size, float(*func)(float, float), float step = 0.1)
	{
		vector<Vertex> vertexArray;
		vector<unsigned int> indicesArray;

		int sizeX = size.x / step;
		int sizeY = size.y / step;

		// Generate verticies
		for (int i = 0; i < sizeX * sizeY; i++)
		{
			int x = i % sizeX;
			int y = i / sizeX;

			Vertex v;
			v.pos = Vector3(x, func(x, y), y);
			v.UV = Vector2(0, 0);
			v.normal = Vector3(0, 0, 0);

			vertexArray.push_back(v);
		}

		for (int y = 0; y < sizeY - 1; y++)
		{
			for (int x = 0; x < sizeX - 1; x++)
			{
				// Creating 2 triangles to create rectangle
				int ind[6];
				ind[0] = y * sizeX + x;
				ind[1] = (y + 1) * sizeX + x;
				ind[2] = y * sizeX + (x + 1);
				ind[3] = ind[2];
				ind[4] = ind[1];
				ind[5] = (y + 1) * sizeX + (x + 1);

				for (int i = 0; i < 6; i++)
					indicesArray.push_back(ind[i]);

				// Generate normals
				Vector3 n1, n2;

				n1 = CalculateTriangleNormal(vertexArray[ind[0]], vertexArray[ind[1]], vertexArray[ind[2]]);
				n2 = CalculateTriangleNormal(vertexArray[ind[3]], vertexArray[ind[4]], vertexArray[ind[5]]);

				vertexArray[ind[0]].normal += n1;
				vertexArray[ind[1]].normal += n1;
				vertexArray[ind[2]].normal += n1;
				vertexArray[ind[3]].normal += n2;
				vertexArray[ind[4]].normal += n2;
				vertexArray[ind[5]].normal += n2;
			}
		}

		// Normalize normals
		for (int i = 0; i < sizeX * sizeY; i++)
		{
			vertexArray[i].normal.Normalize();
		}

		CopyFrom(vertexArray, indicesArray);
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

	Mesh()
	{
		updateRenderer = true;
		SetMaterial(Material::GetDefaultMaterial());

		InitObjects();
	}

	Mesh(shared_ptr<Material> mat)
	{
		updateRenderer = true;
		SetMaterial(mat);

		InitObjects();
	}

	void __Draw()
	{
		__Draw(true);
	}

	void __Draw(bool useShader)
	{

		// Re-initialize because vertexData is not nullptr
		/*if (!initialized)
		{
			InitObjects();
			CopyFrom(vertexData, vertexDataSize, indices, indicesSize);
			//return;
		}

		if (shader == nullptr)
		{
			LOGW_E("Shader is not definied, setting default shader");
			shader = Shader::GetDefaultShader();
			return;
		}*/

		if (useShader)
		{
			if (!initialized)
			{
				//InitObjects();
				CopyFrom(vertexData, indices);
			}

			if (material == nullptr)
			{
				LOGW_E("Material is not definied, setting Material shader");
				material = Material::GetDefaultMaterial();
			}

			material->shader->Use();
			material->__SendToShader();

			glBindVertexArray(VAO);

			/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);*/
		}

		glm::mat4 rotation = glm::toMat4(GetRotation().GetGlVector());


		glm::mat4 translation = glm::translate(glm::mat4(1.0f), GetPosition().GetGLVector());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetScale().GetGLVector());

		glm::mat4 model = translation * rotation * scale;

		//Vector3 pos = meshes[i][j]->GetPosition();
		material->shader->SetMat4(material->shader->modelLocation, glm::value_ptr(model));

		if (faceCulling != FaceCullingModes::Default)
		{
			glEnable(GL_CULL_FACE);
			long mode = 0;
			switch (faceCulling)
			{
			case Mesh::Front:
				mode = GL_FRONT;
				break;
			case Mesh::Back:
				mode = GL_BACK;
				break;
			case Mesh::FrontAndBack:
				mode = GL_FRONT_AND_BACK;
				break;
			}

			glCullFace(mode);
		}

		// Draw by indices
		if (indices.size() > 0)
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		else // Draw by vertexes
			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexData.size());

		if (faceCulling != FaceCullingModes::Default)
			glDisable(GL_CULL_FACE);
	}

	void OnDestroy()
	{
		cout << "Mesh destructor" << endl;
		

		if (initialized)
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);

			initialized = false;
		}
	}
};