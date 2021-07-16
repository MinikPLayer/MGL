#pragma once

#include "Mesh.h"
#include "Asset.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Asset
{
    bool debug = false;

public:
    Model(const char* path, shared_ptr<Material> overrideMat = nullptr, bool debug = false)
    {
        this->debug = debug;

        if (!loadModel(path, overrideMat))
        {
            LOGE_E("Failed to load model from ", path);
        }

        
    }


    
    /// <summary>
    /// Spawns meshes contained in this model in the world
    /// </summary>
    /// <param name="positionOffset">Position offset</param>
    /// <param name="parent">Meshes parent</param>
    shared_ptr<GameObject> SpawnMesh(shared_ptr<GameObject> parent, Vector3 positionOffset = Vector3(0,0,0), shared_ptr<Material> materialOverride = nullptr);
    shared_ptr<GameObject> SpawnMesh(shared_ptr<GameObject> parent, shared_ptr<Material> materialOverride = nullptr);
    shared_ptr<GameObject> SpawnMesh(shared_ptr<Material> materialOverride = nullptr);

private:

    // model data
    vector<shared_ptr<Mesh>> meshes;
    string directory;

    bool loadModel(string path, shared_ptr<Material>& overrideMat);
    void processNode(aiNode* node, const aiScene* scene, shared_ptr<Material>& overrideMat);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene, shared_ptr<Material>& overrideMat);

    
    void loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};