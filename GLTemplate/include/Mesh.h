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
#include <functional>

using namespace std;
using namespace glm;

struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 UV;
	Vector3 tangent;
	Vector3 bitangent;

	Vertex(Vector3 pos, Vector3 normal, Vector2 UV, Vector3 tangent = Vector3(0,0,0), Vector3 bitangent = Vector3(0,0,0))
	{
		this->pos = pos;
		this->normal = normal;
		this->UV = UV;
		this->tangent = tangent;
		this->bitangent = bitangent;
	}

	Vertex()
	{
		this->pos = Vector3(0, 0, 0);
		this->normal = Vector3(0, 0, 0);
		this->UV = Vector2(0, 0);
		this->tangent = Vector3(0, 0, 0);
	}
};


class Mesh : public GameObject
{
public:
	static bool updateRenderer;

	
protected:
	shared_ptr<bool> refCounter = nullptr;

	//vector<Vertex> vertexData;
	//vector<unsigned int> indices;
public:
	shared_ptr<Vertex> vertexData = nullptr;
	shared_ptr<unsigned int> indices = nullptr;
	int vertexDataSize = 0;
	int indicesDataSize = 0;
protected:

	bool initialized = false;

	bool generatedMesh = false;
	Vector2 generatedMeshSize = Vector2(0, 0);
	float generatedMeshStep = 0;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;

	shared_ptr<Material> material;

	void InitObjects();

	void CopyFromInit();
public:
	enum class FaceCullingModes
	{
		Back,
		Front,
		FrontAndBack,
		Disabled
	};

	FaceCullingModes faceCulling = FaceCullingModes::Back;

	void SetMaterial(shared_ptr<Material> mat);
	shared_ptr<Material> GetMaterial();

	void CopyFrom(Mesh* mesh);
	void CopyFrom(vector<Vertex> vertexData, vector<unsigned int> indices);
	void CopyFrom(vector<Vector3> vertex, vector<Vector3> normals, vector<Vector2> uv, vector<unsigned int> indices);
	void CopyFrom(Vertex* vertexArray, int vSize, unsigned int* indicesArray, int iSize);

	void CalculateMeshTangents();

	void CalculateMeshNormals();
	static Vector3 CalculateTriangleNormal(Vertex vtx1, Vertex vtx2, Vertex vtx3);

	void UpdateGeneratedMesh(std::function<float(float, float)> heightFunc);

	void GenerateMesh(Vector2 size, std::function<float(float, float)> heightFunc, std::function<Vector2(float, float)> uvFunction = nullptr, float step = 0.1);
	Mesh();
	Mesh(shared_ptr<Material> mat);

	~Mesh();

	void __Draw(shared_ptr<Material> materialOverride = nullptr);
};