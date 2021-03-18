#pragma once

#include "GameObject.h"
#include "Mesh.h"
#include "Cube.h"
#include "Color.h"
//#include <glad/glad.h>

class Light : public GameObject
{
public:
	Color color;
	float intensity;

	Light(Color color = Color(255, 255, 255, 255), float intensity = 1);

	Vector3 GetColorVector();
};

class LightCube : public Mesh
{
	static Material* lightMat;
public:
	Light* light;

	LightCube()
	{
		light = AddComponent<Light>();
		light->color = Color::Blue;

		// Debug
		//Mesh::CopyFrom((float*)Cube::vertCube, Cube::vertCubeSize, (unsigned int*)Cube::indicesCube, Cube::indicesCubeSize);
		//Mesh::CopyFrom(Cube::cubeVerticies, Cube::cubeNormals, Cube::cubeUVs, Cube::cubeIndices);
		Mesh::CopyFrom(Cube::cubeVertexData, Cube::cubeIndices);

		if (lightMat == nullptr)
		{
			//lightMat = Material::GetDefaultMaterial();
			lightMat = new Material();
			lightMat->shader = new Shader("VertexLightShader.vert", "FragmentLightShader.frag");
		}

		//SetShader(new Shader("VertexLightShader.vert", "FragmentLightShader.frag"));
		SetMaterial(lightMat);
		material->SetVec3("objectColor", Vector3(1.0, 1.0, 1.0));
		material->SetVec3("lightColor", light->color.ToVector3());
	}
};
