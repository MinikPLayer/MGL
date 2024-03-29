#pragma once

#include "Mesh.h"

class MeshRenderer
{
	static void RenderMeshGroup(vector<Mesh*>& meshes);

public:
	static void RenderMeshes(vector<shared_ptr<GameObject>>& objects);
};