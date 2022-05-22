#include "Cube.h"


const vector<unsigned int> Cube::cubeIndices = {
		0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10,
		8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
};

const vector<Vertex> Cube::cubeVertexData = {
		Vertex(Vector3(-1, -1, -1), Vector3(-1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, -1, 1), Vector3(-1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, 1, 1), Vector3(-1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, 1, -1), Vector3(-1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, 1, -1), Vector3(0, 1, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, 1, 1), Vector3(0, 1, 0), Vector2(0, 0)),
		Vertex(Vector3(1, 1, 1), Vector3(0, 1, 0), Vector2(0, 0)),
		Vertex(Vector3(1, 1, -1), Vector3(0, 1, 0), Vector2(0, 0)),
		Vertex(Vector3(1, 1, -1), Vector3(1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(1, 1, 1), Vector3(1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(1, -1, 1), Vector3(1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(1, -1, -1), Vector3(1, 0, 0), Vector2(0, 0)),
		Vertex(Vector3(1, -1, -1), Vector3(0, -1, 0), Vector2(0, 0)),
		Vertex(Vector3(1, -1, 1), Vector3(0, -1, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, -1, 1), Vector3(0, -1, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, -1, -1), Vector3(0, -1, 0), Vector2(0, 0)),
		Vertex(Vector3(-1, 1, -1), Vector3(0, 0, -1), Vector2(0, 0)),
		Vertex(Vector3(1, 1, -1), Vector3(0, 0, -1), Vector2(0, 0)),
		Vertex(Vector3(1, -1, -1), Vector3(0, 0, -1), Vector2(0, 0)),
		Vertex(Vector3(-1, -1, -1), Vector3(0, 0, -1), Vector2(0, 0)),
		Vertex(Vector3(1, 1, 1), Vector3(0, 0, 1), Vector2(0, 0)),
		Vertex(Vector3(-1, 1, 1), Vector3(0, 0, 1), Vector2(0, 0)),
		Vertex(Vector3(-1, -1, 1), Vector3(0, 0, 1), Vector2(0, 0)),
		Vertex(Vector3(1, -1, 1), Vector3(0, 0, 1), Vector2(0, 0)),
};