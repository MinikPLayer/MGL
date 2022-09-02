#include "Sky.h"
#include "AssetsLoader.h"
#include "TimeUtils.h"
#include "Camera.h"
#include "Util.h"
#include "Window.h"


float yfov_to_xfov(float yfov, float aspect) { 
	yfov = deg2rad(yfov);    
	float xfov = 2.0f * atan(tan(yfov * 0.5f) * aspect);    
	return rad2deg(xfov); 
}

// Why not working??
Sky::Sky()
{
	this->SetMaterial(shared_ptr<Material>(new Material(AssetsLoader::LoadShader("SkyShader.vert", "SkyShader.frag"))));

	this->material->SetFloat("time", []() {
		return 30;
	});

	this->SetScale(Vector3(1, 1, 1));
}

void Sky::Update()
{
}