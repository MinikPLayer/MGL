#include "Model.h"



/*void Model::__Draw()
{
	for (auto mesh : meshes)
		mesh->__Draw();
}*/

bool Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOGE_E("ERROR::ASSIMP::", importer.GetErrorString());
		return false;
	}

	processNode(scene->mRootNode, scene);
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* m = processMesh(mesh, scene);
		meshes.push_back(m);
		AddComponent<Mesh>(m);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertexData;
	vector<unsigned int> indicesData;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.pos = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
		{
			v.UV = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			v.UV = Vector2(0, 0);
		}

		vertexData.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicesData.push_back(face.mIndices[j]);
	}

	Mesh* m = new Mesh();
	m->CopyFrom(vertexData, indicesData);
	m->SetMaterial(Material::GetDefaultMaterial());
	m->SetParent(this);
	//m->SetPosition(GetPosition());
	//m.initialized = false;

	if (debug)
	{
		cout << "const vector<Vertex> vertexData = {" << endl;
		for (int i = 0; i < vertexData.size(); i++)
		{
			cout << "\tVertex(Vector3(" << vertexData[i].pos.x << ", " << vertexData[i].pos.y << ", " << vertexData[i].pos.z <<
				"), Vector3(" << vertexData[i].normal.x << ", " << vertexData[i].normal.y << ", " << vertexData[i].normal.z <<
				"), Vector2(" << vertexData[i].UV.x << ", " << vertexData[i].UV.y << "))," << endl;
		}
		cout << "};" << endl;

		cout << "const vector<unsigned int> indicesData = {";
		for (int i = 0; i < indicesData.size(); i++)
		{
			if (i % 15 == 0)
				cout << endl << "\t";
			cout << indicesData[i] << ", ";
		}
		cout << endl << "};" << endl;
	}

	return m;
}
