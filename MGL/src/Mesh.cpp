#include "Mesh.h"
#include "OBJLoader.h"
#include "FBX/fbxdocument.h"
#include "SystemInfo.h"
#include <thread>

bool Mesh::updateRenderer = false;

void Mesh::InitObjects()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	initialized = true;
}

void Mesh::CopyFromInit()
{
	if (initialized)
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);

		initialized = false;

		InitObjects();
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertexDataSize * sizeof(Vertex), this->vertexData.get(), GL_STATIC_DRAW);

	if (indicesDataSize > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesDataSize * sizeof(unsigned int), this->indices.get(), GL_STATIC_DRAW);
	}

	// Vertex pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Vertex UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
	glEnableVertexAttribArray(2);

	// Tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	// Bitangents
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);
}

void Mesh::SetMaterial(shared_ptr<Material> mat)
{
	this->material = mat;
}

shared_ptr<Material> Mesh::GetMaterial()
{
	return this->material;
}

void Mesh::CopyFrom(Mesh* mesh)
{
	CopyFrom(mesh->vertexData.get(), mesh->vertexDataSize, mesh->indices.get(), mesh->indicesDataSize);
	SetMaterial(mesh->GetMaterial());
}


void Mesh::CopyFrom(vector<Vertex> vertexData, vector<unsigned int> indices)
{
	Vertex* newVertexData = new Vertex[vertexData.size()];
	unsigned int* newIndicesData = new unsigned int[indices.size()];

	for (int i = 0; i < vertexData.size(); i++) {
		newVertexData[i] = vertexData[i];
	}
	this->vertexDataSize = vertexData.size();

	for (int i = 0; i < indices.size(); i++) {
		newIndicesData[i] = indices[i];
	}
	this->indicesDataSize = indices.size();

	this->indices.reset(newIndicesData);
	this->vertexData.reset(newVertexData);

	CopyFromInit();
}

void Mesh::CopyFrom(vector<Vector3> vertex, vector<Vector3> normals, vector<Vector2> uv, vector<unsigned int> indices)
{
	if (vertex.size() != normals.size() || normals.size() != uv.size())
	{
		LOGE("Mismatch in data size");
		return;
	}

	vector<Vertex> vertexes;
	for (int i = 0; i < vertex.size(); i++)
		vertexes.push_back(Vertex(vertex[i], normals[i], uv[i]));

	CopyFrom(vertexes, indices);
}

void Mesh::CopyFrom(Vertex* vertexArray, int vSize, unsigned int* indicesArray, int iSize)
{
	Vertex* newVertexData = new Vertex[vSize];
	unsigned int* newIndicesData = new unsigned int[iSize];

	for (int i = 0; i < vSize; i++) {
		newVertexData[i] = vertexArray[i];
	}
	this->vertexDataSize = vSize;

	for (int i = 0; i < iSize; i++) {
		newIndicesData[i] = indicesArray[i];
	}
	this->indicesDataSize = iSize;

	this->indices.reset(newIndicesData);
	this->vertexData.reset(newVertexData);

	CopyFromInit();
}
//
//void Mesh::CalculateMeshTangents() {
//	if (indicesDataSize == 0 || indicesDataSize % 3 != 0) {
//		LOGE_E("Bad indices count");
//		return;
//	}
//
//	for(int i = 0; i < indicesDataSize; i += 3) {
//		int ind[3] = { (int)indices.get()[i], (int)indices.get()[i + 1], (int)indices.get()[i + 2] };
//		Vertex* v[3];
//
//		for (int j = 0; j < 3; j++) {
//			// Check to eliminate out of bounds reads / writes
//			if (ind[j] < 0 || ind[j] >= vertexDataSize) {
//				LOGE_E("Invalid vertex index");
//				return;
//			}
//
//			v[j] = &vertexData.get()[ind[j]];
//			v[j]->tangent = Vector3(0, 0, 0);
//			v[j]->bitangent = Vector3(0, 0, 0);
//		}
//
//		Vector3 poss[] = { v[0]->pos, v[1]->pos, v[2]->pos };
//		Vector2 uvs[] = { v[0]->UV, v[1]->UV, v[2]->UV };
//
//		auto d1 = poss[1] - poss[0];
//		auto d2 = poss[2] - poss[0];
//
//		auto dv1 = uvs[1] - uvs[0];
//		auto dv2 = uvs[2] - uvs[0];
//
//		float r = 1.0 / (dv1.x * dv2.y - dv1.y * dv2.x);
//
//		for(int i = 0;i<3;i++) {
//			v[i]->tangent += (d1 * dv2.y - d2 * dv1.y) * r;
//			v[i]->bitangent += (d2 * dv1.x - d1 * dv2.x) * r;
//
//			auto temp = v[i]->bitangent.y;
//			v[i]->bitangent.y = v[i]->bitangent.z;
//			v[i]->bitangent.z = temp;
//		}
//	}
//
//	for(int i = 0; i < vertexDataSize; i++) {
//		Vertex* v = &vertexData.get()[i];
//		v->tangent.Normalize();
//		v->bitangent.Normalize();
//	}
//}

 void Mesh::CalculateMeshTangents() {
 	if (indicesDataSize == 0 || indicesDataSize % 3 != 0) {
 		LOGE_E("Bad indices count");
 		return;
 	}

 	for (int i = 0; i < indicesDataSize; i += 3) {
 		int ind[3] = { (int)indices.get()[i], (int)indices.get()[i + 1], (int)indices.get()[i + 2] };
 		Vertex* v[3];

 		for (int j = 0; j < 3; j++) {
 			// Check to eliminate out of bounds reads / writes
 			if (ind[j] < 0 || ind[j] >= vertexDataSize) {
 				LOGE_E("Invalid vertex index");
 				return;
 			}

 			v[j] = &vertexData.get()[ind[j]];
 			v[j]->tangent = Vector3(0, 0, 0);
 			v[j]->bitangent = Vector3(0, 0, 0);
 		}

 		float x1 = v[1]->pos.x - v[0]->pos.x;
         float x2 = v[2]->pos.x - v[0]->pos.x;
 		float y1 = v[1]->pos.y - v[0]->pos.y;
         float y2 = v[2]->pos.y - v[0]->pos.y;
 		float z1 = v[1]->pos.z - v[0]->pos.z;
         float z2 = v[2]->pos.z - v[0]->pos.z;

 		float s1 = v[1]->UV.x - v[0]->UV.x;
 		float s2 = v[2]->UV.x - v[0]->UV.x;
 		float t1 = v[1]->UV.y - v[0]->UV.y;
 		float t2 = v[2]->UV.y - v[0]->UV.y;

 		float r = 1.0F / (s1 * t2 - s2 * t1);
 		Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
         Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

 		for(int j = 0; j < 3; j++) {
 			v[j]->tangent += sdir;
 			v[j]->bitangent += tdir;
 		}
 	}

 	for (int i = 0; i < vertexDataSize; i++) {
 		Vertex* v = &vertexData.get()[i];
 		v->tangent = (v->tangent - v->normal * v->normal.Dot(v->tangent)).Normalized();
 	}
 }

void Mesh::CalculateMeshNormals()
{
	if (indicesDataSize == 0 || indicesDataSize % 3 != 0) {
		LOGE_E("Bad indices count");
		return;
	}

	for (int i = 0; i < indicesDataSize; i += 3) {
		int ind[3] = { (int)indices.get()[i], (int)indices.get()[i + 1], (int)indices.get()[i + 2] };
		Vertex* v[3];

		for (int j = 0; j < 3; j++) {
			// Check to eliminate out of bounds reads / writes
			if (ind[j] < 0 || ind[j] >= vertexDataSize) {
				LOGE_E("Invalid vertex index");
				return;
			}

			v[j] = &vertexData.get()[ind[j]];
		}

		auto ret = CalculateTriangleNormal(*v[0], *v[1], *v[2]);
		for (int j = 0; j < 3; j++) {
			v[j]->normal += ret;
		}
	}

	for (int i = 0; i < vertexDataSize; i++) {
		vertexData.get()[i].normal.Normalize();
	}
}

Vector3 Mesh::CalculateTriangleNormal(Vertex vtx1, Vertex vtx2, Vertex vtx3)
{
	Vector3 v1, v2;
	v1 = vtx2.pos - vtx1.pos;
	v2 = vtx3.pos - vtx1.pos;

	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.x * v2.y - v1.y * v2.x,
		v1.z * v2.z - v1.x * v2.z
	);
}

void Mesh::UpdateGeneratedMesh(std::function<float(float, float)> heightFunc)
{
	if (!generatedMesh) {
		LOGE_E("Cannot use UpdateGeneratedMesh to modify mesh that is not generated internally");
		return;
	}

	GenerateMesh(generatedMeshSize, heightFunc, nullptr, generatedMeshStep);
}

void Mesh::GenerateMesh(Vector2 size, std::function<float(float, float)> heightFunc, std::function<Vector2(float, float)> uvFunc, float step)
{
	int sizeX = size.x / step;
	int sizeY = size.y / step;

	// Generate verticies
	Vertex* vertexArray = new Vertex[sizeX * sizeY];
	unsigned int* indicesArray = new unsigned int[(sizeY - 1) * (sizeX - 1) * 6];

	int vIt = 0;
	int iIt = 0;

	for (int i = 0; i < sizeX * sizeY;i++) {
		float x = (i % sizeX) * step;
		float y = (i / sizeX) * step;

		Vertex v;
		v.pos = Vector3(x, heightFunc(x, y), y);

		if (uvFunc == nullptr) {
			v.UV = Vector2(x, y) / size;
		}
		else {
			v.UV = uvFunc(x, y);
		}
		v.normal = Vector3(0, 0, 0);

		vertexArray[vIt++] = v;
	}

	for (int i = 0; i < (sizeY - 1) * (sizeX - 1); i++)
	{
		int x = i % (sizeY - 1);
		int y = i / (sizeY - 1);

		// Creating 2 triangles to create rectangle
		indicesArray[iIt + 0] = y * sizeX + x;
		indicesArray[iIt + 1] = (y + 1) * sizeX + x;
		indicesArray[iIt + 2] = y * sizeX + (x + 1);
		indicesArray[iIt + 3] = indicesArray[iIt + 2];
		indicesArray[iIt + 4] = indicesArray[iIt + 1];
		indicesArray[iIt + 5] = (y + 1) * sizeX + (x + 1);


		// Generate normals
		Vector3 n1, n2;

		n1 = CalculateTriangleNormal(vertexArray[indicesArray[iIt + 0]], vertexArray[indicesArray[iIt + 1]], vertexArray[indicesArray[iIt + 2]]);
		n2 = CalculateTriangleNormal(vertexArray[indicesArray[iIt + 3]], vertexArray[indicesArray[iIt + 4]], vertexArray[indicesArray[iIt + 5]]);

		vertexArray[indicesArray[iIt + 0]].normal += n1;
		vertexArray[indicesArray[iIt + 1]].normal += n1;
		vertexArray[indicesArray[iIt + 2]].normal += n1;
		vertexArray[indicesArray[iIt + 3]].normal += n2;
		vertexArray[indicesArray[iIt + 4]].normal += n2;
		vertexArray[indicesArray[iIt + 5]].normal += n2;

		iIt += 6;
	}

	vertexArray[0].normal = vertexArray[1].normal;

	// Normalize normals
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		vertexArray[i].normal.Normalize();
	}

	CopyFrom(vertexArray, vIt, indicesArray, iIt);
    CalculateMeshNormals();
	CalculateMeshTangents();
    CopyFromInit();

	this->generatedMesh = true;
	this->generatedMeshSize = size;
	this->generatedMeshStep = step;

	delete[] vertexArray;
	delete[] indicesArray;
	
}

Mesh::Mesh()
{
	updateRenderer = true;
	SetMaterial(Material::GetDefaultMaterial());

	InitObjects();
}

Mesh::Mesh(shared_ptr<Material> mat)
{
	updateRenderer = true;
	SetMaterial(mat);

	InitObjects();
}

void Mesh::__Draw(shared_ptr<Material> mat)
{
	if (mat == nullptr)
	{
		if (material == nullptr)
		{
			LOGW_E("Material is not definied, setting Material shader");
			mat = Material::GetDefaultMaterial();
		}
		else {
			mat = material;
		}
	}

	if (!initialized)
		CopyFromInit();

	auto shader = mat->GetShader();
	shader->Use();
	mat->__SendToShader();

	glBindVertexArray(VAO);
	

	glm::mat4 rotation = glm::toMat4(GetRotation().GetGlVector());
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), GetPosition().GetGLVector());
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetScale().GetGLVector());
	glm::mat4 model = translation * rotation * scale;

	shader->SetMat4(shader->modelLocation, glm::value_ptr(model));

	glEnable(GL_CULL_FACE);
	long mode = GL_BACK;
	switch (faceCulling)
	{
	case FaceCullingModes::Disabled:
		glDisable(GL_CULL_FACE);
		break;

	case FaceCullingModes::Front:
		mode = GL_FRONT;
		break;
	case FaceCullingModes::Back:
		mode = GL_BACK;
		break;
	case FaceCullingModes::FrontAndBack:
		mode = GL_FRONT_AND_BACK;
		break;
	}

	glCullFace(mode);

	// Draw by indices
	if (indicesDataSize > 0)
		glDrawElements(GL_TRIANGLES, indicesDataSize , GL_UNSIGNED_INT, 0);
	else // Draw by vertexes
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexDataSize);


	glDisable(GL_CULL_FACE);
}

Mesh::~Mesh()
{
	if (initialized)
	{
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);

		initialized = false;
	}
}