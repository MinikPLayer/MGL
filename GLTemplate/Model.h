#pragma once

#include "Mesh.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public GameObject
{
    bool debug = false;

public:
    Model(const char* path, bool debug = false)
    {
        this->debug = debug;

        if (!loadModel(path))
        {
            LOGE_E("Failed to load model from ", path);
        }

        
    }
    //void Draw(Shader& shader);
private:
    //void __Draw();

    // model data
    vector<Mesh*> meshes;
    string directory;

    bool loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    /*vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);*/
};