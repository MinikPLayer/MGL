#include "EngineUtil.h"
#include "Camera.h"
#include "Mesh.h"

void RunEvent(void (GameObject::* callback)())
{
	for (int i = 0; i < GameObject::__objects.size(); i++)
	{
		if (GameObject::__objects[i] == nullptr)
			continue;

		(GameObject::__objects[i].get()->*(callback))();
	}
}

void RenderScene(Camera* cam, shared_ptr<Material> matOverride, bool forceFrontCull)
{
	if (cam == nullptr) {
		cam = Camera::GetMainCamera();
	}

	//Camera::__SetRenderingCamera(cam);

	// RunEvent
	for (int i = 0; i < GameObject::__objects.size(); i++)
	{
		auto object = GameObject::__objects[i].get();
		if (object == nullptr)
			continue;

		object->__Draw(matOverride);
	}
}
