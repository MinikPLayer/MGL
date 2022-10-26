//
// Created by minik on 26.10.22.
//

#include "Benchmark.h"
#include "../Camera.h"
#include "../AssetsLoader.h"
#include "../Color.h"
#include "../Cameras/FlybackCamera.h"
#include "../SceneManager.h"

void Benchmark::Start() {
    GameObject::Start();

    cubeShader = AssetsLoader::LoadShader("VertexShader.vert", "FragmentShader.frag");
    auto cam = (FlybackCamera*)Camera::GetMainCamera();
    cam->SetPosition(Vector3(-50, 50, 50));
    cam->sensitivity = 0;

    this->fpsTimer = Time::elapsedTime;
}


void Benchmark::Update() {
    GameObject::Update();

    if(--cubeSpawnTimer <= 0 && spawnActive) {
        SpawnNextCube();
        cubeSpawnTimer = 1;
    }

    auto curTime = Time::elapsedTime;
    if((spawnActive && curTime - fpsTimer >= 0.5) || (!spawnActive && curTime - fpsTimer >= 2.0)) {
        auto fps = (float)fpsTicks / (curTime - fpsTimer);
        LOG_E("FPS: ", fps, " - ", this->components.size(), " cubes spawned");
        fpsTicks = 0;
        fpsTimer = curTime;

        if(fps < 60) {
            spawnActive = false;
            LOG_E("Result: ", this->components.size());
            SceneManager::LoadDefaultScene();
            GameObject::Instantiate<Benchmark>();
        }
    }
    else {
        fpsTicks++;
    }
}

const int cubeMaxDist = 100;
void Benchmark::SpawnNextCube() {
    auto newCube = new MovingCube();
    newCube->SetLocalPosition(Vector3::Random(0, cubeMaxDist));
    newCube->SetParent(this);

    auto newCubeMat = shared_ptr<Material>(Material::CreatePrefabedMaterial(Material::SILVER, cubeShader));
    newCubeMat->SetVec3("dirLight.direction", Vector3(0.5f, 0.3f, 0.2f));
    newCubeMat->SetVec3("dirLight.ambient", Vector3(0.05f, 0.05f, 0.05f));
    newCubeMat->SetVec3("dirLight.diffuse", Vector3(0.4f, 0.4f, 0.4f));
    newCubeMat->SetVec3("dirLight.specular", Vector3(0.5f, 0.5f, 0.5f));
    newCubeMat->SetFloat("dirLight.strength", 1.0f);
    newCube->SetMaterial(newCubeMat);

    this->AddComponent(newCube);
}

void MovingCube::Update() {
    GameObject::Update();
    this->Move(Vector3(1, 1, 1) * (float)moveSpeed * Time::deltaTime);

    auto lPos = this->GetLocalPosition();
    if(lPos.x > cubeMaxDist)
        lPos.x = 0;

    if(lPos.y > cubeMaxDist)
        lPos.y = 0;

    if(lPos.z > cubeMaxDist)
        lPos.z = 0;

    this->SetLocalPosition(lPos);
}
