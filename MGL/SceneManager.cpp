//
// Created by minik on 26.10.22.
//

#include "SceneManager.h"
#include "Cameras/FlybackCamera.h"
#include "Sky.h"

void SceneManager::LoadDefaultScene(bool includeSky) {
    GameObject::__objects.clear();

    auto camera = new FlybackCamera();
    camera->SetPosition(Vector3(-3, 0, 0));
    GameObject::Instantiate(camera);
    camera->SetAsMainCamera();

    if(includeSky) {
        sky = new Sky();
        sky->SetPosition(Vector3(0, 0, 0));
        GameObject::Instantiate(sky);
    }
}
