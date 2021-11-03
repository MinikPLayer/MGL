#include "ShadowMap.h"
#include "Camera.h"
#include "EngineUtil.h"
#include "Window.h"
#include "Camera.h"
#include "AssetsLoader.h"
#include "PlaneMesh.h"
#include "Sky.h"
#include "Util.h"

#include <glm/glm.hpp>
#include "Cube.h"


Camera* ShadowMap::shadowMapCamera = new Camera();
vector<ShadowMap*> ShadowMap::__ShadowMaps__;
shared_ptr<Shader> ShadowMap::shadowMapShader = nullptr;

ShadowMap::ShadowMap(GLuint width, GLuint height)
{
	const bool color = false;

	// Create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	if(color)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borders[] = { 1, 1, 1, 1 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borders);

	// Bind underlaying framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	if (color) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthMap, 0);
	}
	else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	
	// Reset to default FB
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	size.x = width;
	size.y = height;

	__ShadowMaps__.push_back(this);
}

ShadowMap::~ShadowMap()
{
	for (size_t i = 0; i < __ShadowMaps__.size(); i++) {
		if (__ShadowMaps__[i] == this) {
			__ShadowMaps__.erase(__ShadowMaps__.begin() + i, __ShadowMaps__.begin() + i + 1);
			return;
		}
	}
}

void ShadowMap::Render()
{
	if (shadowMapShader == nullptr) {
		shadowMapShader = AssetsLoader::LoadShader("ShadowMapVertexShader.vert", "ShadowMapFragmentShader.frag");
	}

	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glClear(GL_DEPTH_BUFFER_BIT);


	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	Vector3 pos = Vector3(0, 0, 0);
	if (posFunc != nullptr)
		pos = posFunc();

	//glm::mat4 lightProjection = glm::ortho(-40.0f, 40.0f, -20.0f, 20.0f, nearPlane, farPlane);
	glm::mat4 lightProjection = glm::perspective(1.8f, 2.f, nearPlane, farPlane);
	//glm::mat4 lightView = glm::lookAt((pos).GetGLVector(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 lightView = glm::translate(glm::mat4(1.0f), -pos.GetGLVector());

	lightSpaceMatrix = lightProjection * lightView;
	shared_ptr<Material> mat = shared_ptr<Material>(new Material(shadowMapShader));
	shadowMapShader->SetMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
	shadowMapShader->Use();

	Material::disableTextures = true;
	Shader::prohibitShaderChange = true;
	RenderScene(nullptr, mat, true);
	Shader::prohibitShaderChange = false;
	Material::disableTextures = false;

	//delete testCube;

	// Reset FB
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Window::GetMainWindow()->SetViewport();
}

void ShadowMap::DebugDraw()
{	
	shared_ptr<Shader> shader = AssetsLoader::LoadShader("DepthMapShader.vert", "DepthMapShader.frag");

	PlaneMesh m;
	shared_ptr<Material> mat = shared_ptr<Material>(new Material(shader));

	m.SetMaterial(mat);
	m.faceCulling = Mesh::FaceCullingModes::Disabled;

	glActiveTexture(GL_TEXTURE0);
	mat->shader->SetInt("depthMap", 0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glDisable(GL_DEPTH_TEST);
	m.__Draw();
	glEnable(GL_DEPTH_TEST);
}
