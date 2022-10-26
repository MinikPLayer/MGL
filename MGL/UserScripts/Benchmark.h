//
// Created by minik on 26.10.22.
//

#ifndef MGL_BENCHMARK_H
#define MGL_BENCHMARK_H

#include "../GameObject.h"
#include "../Cube.h"
#include <vector>

class MovingCube : public Cube {
public:
    float moveSpeed = 10;

    void Update() override;
};

class Benchmark : public GameObject {
    bool spawnActive = true;
    int cubeSpawnTimer = 0;
    float fpsTimer = 0;
    int fpsTicks = 0;
    shared_ptr<Shader> cubeShader;

    void SpawnNextCube();
public:
    void Update() override;
    void Start() override;
};


#endif //MGL_BENCHMARK_H
