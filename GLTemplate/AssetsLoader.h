#pragma once
#include "Asset.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include <vector>
#include <memory>

using namespace std;


class AssetsLoader
{
	static vector<weak_ptr<Asset>> loadedAssets;

	template<class T>
	static bool FindAsset(string path, shared_ptr<T>& foundAsset);

	template<class T>
	static void AddAsset(string path, shared_ptr<T>& asset);
public:
	//static Asset<Shader>& LoadShader(const char* vertexShader, const char* fragmentShader);
	static shared_ptr<Shader> LoadShader(const char* vertexShader, const char* fragmentShader);
	static shared_ptr<Model> LoadModel(string path, shared_ptr<Material> mat = nullptr);
	static shared_ptr<Texture> LoadTexture(string path, int slot = 0);
};

template<class T>
inline bool AssetsLoader::FindAsset(string path, shared_ptr<T>& foundAsset)
{
	for (int i = 0; i < loadedAssets.size(); i++)
	{
		// If expired then just remove it from vector
		if (loadedAssets[i].expired())
		{
			loadedAssets.erase(loadedAssets.begin() + i, loadedAssets.begin() + i + 1);
			i--;
			continue;
		}

		shared_ptr<Asset> ptr = loadedAssets[i].lock();

		// Must be the same type and name
		if (ptr->IsAssetType<T>() && ptr->path == path)
		{
			foundAsset = static_pointer_cast<T>(ptr);
			return true;
		}
			



	}

	return false;
}

template<class T>
inline void AssetsLoader::AddAsset(string path, shared_ptr<T>& asset)
{
	asset->__SetAssetType<T>();
	asset->path = path;

	loadedAssets.push_back(static_pointer_cast<Asset>(asset));
}
