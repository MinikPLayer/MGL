#include "Mesh.h"
#include "OBJLoader.h"
#include "FBX/fbxdocument.h"

bool Mesh::updateRenderer = false;

/*bool Mesh::LoadFromObj(string filePath)
{
	objl::Loader loader;

	bool result = loader.LoadFile(filePath);
	if (!result)
		return false;
	
	int size = loader.LoadedVertices.size();
	//float* vert = new float[size];
	vector<Vertex> vert(size);

	for (int i = 0; i < size; i+=3)
	{
		//vert[i] = loader.LoadedVertices[i/3].Position.X;
		//vert[i + 1] = loader.LoadedVertices[i/3].Position.Y;
		//vert[i + 2] = loader.LoadedVertices[i/3].Position.Z;
		vert[i].pos = Vector3(loader.LoadedVertices[i].Position.X, loader.LoadedVertices[i].Position.Y, loader.LoadedVertices[i].Position.Z);
	}

	//CopyFrom(vert, size, &loader.LoadedIndices[0], loader.LoadedIndices.size());
	CopyFrom(vert, loader.LoadedIndices);

	return true;
}

bool Mesh::LoadFromFBX(string filePath)
{
	fbx::FBXDocument file;
	file.read(filePath);
	LOG("Nodes found: ", file.nodes.size());

	for (auto node : file.nodes)
	{
		LOG("\t", node.getName());
		if (node.getName() == "objects")
		{
			string data;
			
		}
	}

	return false;
}*/


