#include "Sky.h"
#include "AssetsLoader.h"
#include "Time.h"
#include "Camera.h"

Sky::Sky()
{
	this->material = shared_ptr<Material>(new Material(AssetsLoader::LoadShader("SkyShader.vert", "SkyShader.frag")));
	this->material->SetVec2("_resolution_", Vector2(1920, 1080));
	//this->material->SetVec2("sunPos", Vector2(1700, 800));
	this->material->SetVec2("sunPos", []() {
		//return Vector2(sin(Time::elapsedTime) * 800 + 800, cos(Time::elapsedTime) * 800 + 400);
		return Vector2(800, 600);
	});
	this->faceCulling = FaceCullingModes::Front;

	this->SetScale(Vector3(1, 1, 1));
}

void Sky::Update()
{
	this->position = Camera::GetMainCamera()->GetPosition();	
}