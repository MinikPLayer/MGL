#pragma once

#include "Mesh.h"

class Cube : public Mesh
{
public:
	const static vector<Vertex> cubeVertexData;
	const static vector<unsigned int> cubeIndices;

	void SetMaterial(shared_ptr<Material> mat)
	{
		//Mesh::SetShader(s);
		Mesh::SetMaterial(mat);
	}

	Cube()
	{
		Mesh::CopyFrom(cubeVertexData, cubeIndices);
	}
};