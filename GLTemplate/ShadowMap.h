#pragma once
#include "Vector.h"
#include "Framebuffer.h"
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include <memory>

class ShadowMap : public Framebuffer {

	static Camera* shadowMapCamera;
	static shared_ptr<Shader> shadowMapShader;
	GLuint depthMap;

	Vector2i size;

	bool orto;

	std::shared_ptr<Material> mat;

public:


	glm::mat4 lightSpaceMatrix;
	GLuint GetDepthMapID() { return depthMap; }

	function<Vector3()> posFunc;

	// List of shadow maps, used for rendering
	static std::vector<ShadowMap*> __ShadowMaps__;

	ShadowMap(bool ortographic, GLuint width = 512, GLuint height = 512);
	~ShadowMap();

	void Render();

	void DebugDraw();
};

