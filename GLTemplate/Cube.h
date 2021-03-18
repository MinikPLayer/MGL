#pragma once

#include "Mesh.h"

class Cube : public Mesh
{
public:
	//const static int vertCubeSize = 24;//180;
	//const static float vertCube[];
	//const static unsigned int indicesCube[];
	//const static int indicesCubeSize = 36;
	//const static vector<Mesh::Vertex> vertCube[];
	//const static vector<Vector3> cubeVerticies;
	//const static vector<Vector3> cubeNormals;
	//const static vector<Vector2> cubeUVs;
	const static vector<Vertex> cubeVertexData;
	const static vector<unsigned int> cubeIndices;


	/*static float* GetVertexData()
	{
		return (float*)vertCube;
	}

	static int GetVertexDataSize()
	{
		return vertCubeSize;
	}*/

	//void SetShader(Shader* s)
	void SetMaterial(Material* mat)
	{
		//Mesh::SetShader(s);
		Mesh::SetMaterial(mat);
	}

	Cube()
	{
		//vertexData = (float*)vertCube;
		//vertexDataSize = vertCubeSize;

		//Mesh::CopyFrom((float*)vertCube, vertCubeSize, (unsigned int*)indicesCube, indicesCubeSize);
		//Mesh::CopyFrom(cubeVerticies, cubeNormals, cubeUVs, cubeIndices);
		Mesh::CopyFrom(cubeVertexData, cubeIndices);
	}

	void Update()
	{
		//cout << "Cube update, vertex data size: " << vertexDataSize << endl;
	}
};

/*const vector<unsigned int> Cube::cubeIndices =
{
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};*/


/*const vector<Vector3> Cube::cubeVerticies =
{
	Vector3(-1, -1, -1),
	Vector3(1, -1, -1),
	Vector3(1, 1, -1),
	Vector3(-1, 1, -1),
	Vector3(-1, -1, 1),
	Vector3(1, -1, 1),
	Vector3(1, 1, 1),
	Vector3(-1, 1, 1)
};

const vector<Vector2> Cube::cubeUVs =
{
	Vector2(0, 0),
	Vector2(1, 0),
	Vector2(1, 1),
	Vector2(0, 1)
};

const vector<Vector3> Cube::cubeNormals =
{
	Vector3(0, 0, 1),
	Vector3(1, 0, 0),
	Vector3(0, 0, -1),
	Vector3(-1, 0, 0),
	Vector3(0, 1, 0),
	Vector3(0, -1, 0)
};*/

/*const vector<Mesh::Vertex> Cube::vertCube[] = {
	-1, -1, -1,
	1, -1, -1,
	1, 1, -1,
	-1, 1, -1,
	-1, -1, 1,
	1, -1, 1,
	1, 1, 1,
	-1, 1, 1
};

const unsigned int Cube::indicesCube[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};*/
/*const float Cube::vertCube[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
const unsigned int Cube::indicesCube[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};*/

/*const float Cube::vertCube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};*/