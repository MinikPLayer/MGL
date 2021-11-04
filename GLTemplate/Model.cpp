#include "Model.h"
#include "Path.h"
#include "AssetsLoader.h"
#include "Debug.h"

/*void Model::__Draw()
{
	for (auto mesh : meshes)
		mesh->__Draw();
}*/

bool Model::loadModel(string path, shared_ptr<Material>& overrideMat)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOGE_E("ERROR::ASSIMP::", importer.GetErrorString());
		return false;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of(Path::SEPARATOR));

	processNode(scene->mRootNode, scene, overrideMat);
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene, shared_ptr<Material>& overrideMat)
{

	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* m = processMesh(mesh, scene, overrideMat);
		//meshes.push_back(ptr);
		//shared_ptr<Mesh> ptr = AddComponent(m);
		shared_ptr<Mesh> ptr(m);
		meshes.push_back(ptr);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, overrideMat);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene, shared_ptr<Material>& overrideMat)
{
	vector<Vertex> vertexData;
	vector<unsigned int> indicesData;
	vector<Vector3> tangents;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{

		Vertex v;
		v.pos = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->HasTangentsAndBitangents())
		{
			v.tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			v.bitangent = Vector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
		{
			LOGW_E("No tangents and bitangents in mesh");
			v.tangent = Vector3(0, 0, 0);
			v.bitangent = Vector3(0, 0, 0);
		}

		if (mesh->HasTextureCoords(0))
		{
			v.UV = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			v.UV = Vector2(0, 0);
			LOG("No vertex UV");
		}

		vertexData.push_back(v);

	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicesData.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		LOG_E("Loaded textures");
	}

	Mesh* m = new Mesh();
	m->CopyFrom(vertexData, indicesData);
	if (overrideMat != nullptr)
		m->SetMaterial(overrideMat);
	else
		m->SetMaterial(Material::GetDefaultMaterial());
	//m->SetParent(this);
	
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

void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{

	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		//shared_ptr<Texture> newTex(new Texture(Path::Combine({ directory, str.C_Str() }).c_str(), 0));
		int slot = -1;
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			slot = 0;
			break;

		case aiTextureType_SPECULAR:
		case aiTextureType_AMBIENT:
		case aiTextureType_EMISSIVE:
		case aiTextureType_HEIGHT:
		case aiTextureType_NORMALS:
		case aiTextureType_SHININESS:
		case aiTextureType_OPACITY:
		case aiTextureType_DISPLACEMENT:
		case aiTextureType_LIGHTMAP:
		case aiTextureType_REFLECTION:	
		case _aiTextureType_Force32Bit:
			LOGW_E("Unsupported texture type");
			break;
		case aiTextureType_NONE:
		case aiTextureType_UNKNOWN:
		default:
			LOGW_E("Trying to load none or unknown texture type");
			break;
		}

		if (slot < 0)
			continue;

		string p = Path::Combine({ directory, str.C_Str() });
		LOG("TexturePath: ", p);
		// Load texture into assets
		AssetsLoader::LoadTexture(p, slot, true);
	}
}

shared_ptr<GameObject> Model::SpawnMesh(shared_ptr<GameObject> parent, shared_ptr<Material> materialOverride)
{
	return SpawnMesh(parent, Vector3(0, 0, 0), materialOverride);
}

shared_ptr<GameObject> Model::SpawnMesh(shared_ptr<Material> materialOverride)
{
	return SpawnMesh(nullptr, Vector3(0, 0, 0), materialOverride);
}

shared_ptr<GameObject> Model::SpawnMesh(shared_ptr<GameObject> parent, Vector3 positionOffset, shared_ptr<Material> materialOverride)
{
	// Create root object if needed
	if (parent == nullptr)
		parent = GameObject::Instantiate<GameObject>();

	for (int i = 0; i < meshes.size(); i++)
	{
		shared_ptr<Mesh> m = parent->AddComponent<Mesh>();

		m->SetLocalPosition(positionOffset);
		m->CopyFrom(meshes[i].get());

		if (materialOverride != nullptr)
			m->SetMaterial(materialOverride);
	}

	return parent;
}
