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
	// TODO: Implement asset garbage collector
	static vector<shared_ptr<Asset>> loadedAssets;

public:

	static shared_ptr<Shader> LoadShader(const char* vertexShader, const char* fragmentShader, bool absoluteDir = false);
	static shared_ptr<Model> LoadModel(string path, shared_ptr<Material> mat = nullptr, bool absoluteDir = false);
	static shared_ptr<Texture> LoadTexture(string path, int slot = 0, bool absoluteDir = false);


	template<class T>
	static void AddAsset(string path, shared_ptr<T>& asset);


	template<class T>
	static bool FindAsset(string path, shared_ptr<T>& foundAsset);
};

template<class T>
inline bool AssetsLoader::FindAsset(string path, shared_ptr<T>& foundAsset)
{
	for (int i = 0; i < loadedAssets.size(); i++)
	{
		// TODO: If expired then just remove it from vector
		/*if (loadedAssets[i].expired())
		{
			loadedAssets.erase(loadedAssets.begin() + i, loadedAssets.begin() + i + 1);
			i--;
			continue;
		}*/

		// Must be the same type and name
		if (loadedAssets[i]->IsAssetType<T>() && loadedAssets[i]->path == path)
		{
			foundAsset = static_pointer_cast<T>(loadedAssets[i]);
			return true;
		}

	}

	return false;
}

template<class T>
inline void AssetsLoader::AddAsset(string path, shared_ptr<T>& asset)
{
	((Asset*)asset.get())->__SetAssetType<T>();
	asset->path = path;

	loadedAssets.push_back(static_pointer_cast<Asset>(asset));
}
