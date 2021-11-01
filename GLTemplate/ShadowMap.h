#pragma once
#include "Vector.h"
#include "Framebuffer.h"
#include <vector>
#include "Camera.h"
#include "Shader.h"

class ShadowMap : public Framebuffer {

	static Camera* shadowMapCamera;
	static shared_ptr<Shader> shadowMapShader;
	GLuint depthMap;

	Vector2i size;
public:
	// List of shadow maps, used for rendering
	static std::vector<ShadowMap*> __ShadowMaps__;

	ShadowMap(GLuint width = 1024, GLuint height = 1024);
	~ShadowMap();

	Vector3 pos;

	void Render();

	void DebugDraw();
};

