#include "AssetsLoader.h"
#include "Util.h"
#include "Path.h"

vector<weak_ptr<Asset>> AssetsLoader::loadedAssets;


shared_ptr<Shader> AssetsLoader::LoadShader(const char* vertexShader, const char* fragmentShader)
{
	string path = Path::Combine({ Path::ASSETS_PATH, "Shaders" });
	shared_ptr<Shader> asset;

	string assetPath = Path::Combine({ path, vertexShader, fragmentShader });
	// Asset already instantiated
	if (FindAsset<Shader>(assetPath, asset))
		return asset;

	asset = shared_ptr<Shader>(
		new Shader(
			Path::Combine({ path, vertexShader }), 
			Path::Combine({ path, fragmentShader })
		)
	);
	AddAsset<Shader>(assetPath, asset);

	return asset;
}

shared_ptr<Model> AssetsLoader::LoadModel(string path, shared_ptr<Material> mat)
{
	string p = Path::Combine({ Path::ASSETS_PATH, "Models", path });
	shared_ptr<Model> asset;

	// Asset already instatiated
	if (FindAsset<Model>(p, asset))
	{
		return asset;
	}

	asset = shared_ptr<Model>(new Model(p.c_str(), mat));
	AddAsset<Model>(p, asset);

	return asset;
}

shared_ptr<Texture> AssetsLoader::LoadTexture(string path, int slot)
{
	string p = Path::Combine({ Path::ASSETS_PATH, "Textures", path });
	shared_ptr<Texture> asset;

	// Asset already instatiated
	if (FindAsset<Texture>(p, asset))
		return asset;

	asset = shared_ptr<Texture>(new Texture(p.c_str(), slot));
	AddAsset<Texture>(p, asset);

	return asset;
}

