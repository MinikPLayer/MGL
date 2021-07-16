#include "AssetsLoader.h"
#include "Util.h"
#include "Path.h"

vector<shared_ptr<Asset>> AssetsLoader::loadedAssets;


shared_ptr<Shader> AssetsLoader::LoadShader(const char* vertexShader, const char* fragmentShader, bool absoluteDir)
{
	string path = "";
	string assetPath = "";

	if (!absoluteDir)
		path = Path::Combine({ Path::ASSETS_PATH, "Shaders" });

	shared_ptr<Shader> asset;

	assetPath = Path::Combine({ path, vertexShader, fragmentShader });
	
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

shared_ptr<Model> AssetsLoader::LoadModel(string path, shared_ptr<Material> mat, bool absoluteDir)
{
	if(!absoluteDir)
		path = Path::Combine({ Path::ASSETS_PATH, "Models", path });
	
	shared_ptr<Model> asset;

	// Asset already instatiated
	if (FindAsset<Model>(path, asset))
	{
		return asset;
	}

	asset = shared_ptr<Model>(new Model(path.c_str(), mat));
	AddAsset<Model>(path, asset);

	return asset;
}

shared_ptr<Texture> AssetsLoader::LoadTexture(string path, int slot, bool absoluteDir)
{
	if(!absoluteDir)
		path = Path::Combine({ Path::ASSETS_PATH, "Textures", path });
	
	shared_ptr<Texture> asset;

	// Asset already instatiated
	if (FindAsset<Texture>(path, asset))
		return asset;

	asset = shared_ptr<Texture>(new Texture(path.c_str(), slot));
	AddAsset<Texture>(path, asset);

	return asset;
}

