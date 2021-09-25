#include "Sky.h"
#include "AssetsLoader.h"
#include "Time.h"
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

	vector<Vector3> v = {
		Vector3(-1, -1, -1),
		Vector3(1, -1, -1),
		Vector3(1, 1, -1),
		Vector3(-1, 1, -1),
	};

	vector<unsigned int> ind = {
		0, 1, 2, 0, 2, 3
	};

	vector<Vector2> nullVector;
	vector<Vector3> nullVector3;
	for (int i = 0; i < 4; i++) {
		nullVector.push_back(Vector2(0, 0));
		nullVector3.push_back(Vector3(0, 0, 0));
	}


	this->CopyFrom(v, nullVector3, nullVector, ind);
	this->SetMaterial(shared_ptr<Material>(new Material(AssetsLoader::LoadShader("SkyShader.vert", "SkyShader.frag"))));

	this->material->SetFloat("time", []() {
		//return Time::elapsedTime;
		return 30;
	});

	this->SetScale(Vector3(1, 1, 1));
}

void Sky::Update()
{
	//this->position = Camera::GetMainCamera()->GetPosition();	
}