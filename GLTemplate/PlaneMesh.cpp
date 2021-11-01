#include "PlaneMesh.h"

PlaneMesh::PlaneMesh()
	:Mesh::Mesh()
{

	const float end = 1;
	Vertex v[4] = {
		Vertex(
			Vector3(-1, -1, 0),
			Vector2(0,0),
			Vector2(0, 0)
		),
		Vertex(
			Vector3(1, -1, 0),
			Vector2(0, 0),
			Vector2(end, 0)
		),
		Vertex(
			Vector3(1, 1, 0),
			Vector2(0, 0),
			Vector2(end, end)
		),
		Vertex(
			Vector3(-1, 1, 0),
			Vector2(0, 0),
			Vector2(0, end)
		)
	};

	unsigned int ind[6] = {
		0, 1, 2, 0, 2, 3
	};

	this->CopyFrom(v, 4, ind, 6);
	this->CalculateMeshNormals();
}
