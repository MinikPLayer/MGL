#include "Sky.h"
#include "AssetsLoader.h"
#include "Time.h"

void Sky::__Draw()
{
	glDisable(GL_DEPTH_TEST);

	Mesh::__Draw();

	glEnable(GL_DEPTH_TEST);
}

Sky::Sky()
	:Mesh(shared_ptr<Material>(new Material()))
{
	this->material->shader = AssetsLoader::LoadShader("SkyShader.vert", "SkyShader.frag");

	// Generate plane
	CopyFrom(vector<Vertex>({
		Vertex(Vector3(0,0,0), Vector3(0,0,0), Vector2(0,0)),
		Vertex(Vector3(0,0,0), Vector3(0,0,0), Vector2(0,0)),
		Vertex(Vector3(0,0,0), Vector3(0,0,0), Vector2(0,0)),
		Vertex(Vector3(0,0,0), Vector3(0,0,0), Vector2(0,0)),
	}), vector<unsigned int>({}) // Emtpy indicies so we draw by vertex
	);
}

void Sky::Update()
{
	//Mesh::Update();

	this->material->SetFloat("time", Time::elapsedTime * 5 );

}
