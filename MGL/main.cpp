#include <iostream>

#include "glad/glad.h"

#include "AssetsLoader.h"
#include "Util.h"
#include "EngineUtil.h"
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "UserScript.h"
#include "TimeUtils.h"
#include "Input.h"
#include "Vector.h"
#include "Window.h"
#include "Cameras/FlybackCamera.h"
#include "ImGui/imgui.h"
#include "SystemInfo.h"
#include "PostProcessing.h"
#include "SceneManager.h"

using namespace std;

float texMix = 0.2f;
float window_width = 1280;
float window_height = 720;

float lastFrame = 0.0f;

Camera* camera;

void registerBasicInput()
{
	Input::RegisterAxis(Input::Axis("CloseWindow", Input::Devices::Keyboard, Input::ESCAPE, Input::NONE));
	Input::RegisterAxis(Input::Axis("Horizontal", Input::Keyboard, Input::D, Input::A));
	Input::RegisterAxis(Input::Axis("Vertical", Input::Keyboard, Input::W, Input::S));
	Input::RegisterAxis(Input::Axis("MouseX", Input::Mouse, Input::MouseAxis::MOUSE_X));
	Input::RegisterAxis(Input::Axis("MouseY", Input::Mouse, Input::MouseAxis::MOUSE_Y));
	Input::RegisterAxis(Input::Axis("Sprint", Input::Keyboard, Input::LSHIFT));
}

int main()
{
	srand(time(NULL));

	ImGui::CreateContext();
	
	Window window(Vector2(window_width, window_height), "LearnOpenGL", true);
	
	// Setup input
	window.SetCursorMode(Window::CursorModes::Disabled);

	// Disable / Enabke VSYNC
	glfwSwapInterval(0);
    glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0, 0, 0, 1);

	registerBasicInput();

	LOG("Processor count: ", SystemInfo::GetThreadCount());

    SceneManager::LoadDefaultScene();
    GameObject::Instantiate<UserScript>();

	LOG_E("Starting drawing loop...");

	bool dynamicBatching = true;
	Input::RegisterAxis(Input::Axis("DynBatch", Input::Keyboard, Input::C));

	PostProcessing pp(window.GetSize());

	while(!window.ShouldClose())
	{
		float currentFrame = glfwGetTime();
		Time::deltaTime = currentFrame - lastFrame;
		Time::elapsedTime = currentFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.PollEvents();

		RunEvent(&GameObject::Update);

		Camera::__SetRenderingCamera(Camera::GetMainCamera(), window.GetAspectRatio());

		// Render shadow maps
		for (int i = 0; i < ShadowMap::__ShadowMaps__.size(); i++) {
			ShadowMap::__ShadowMaps__[i]->Render();
		}

		Camera::__SetRenderingCamera(Camera::GetMainCamera());
			
		pp.Use();
		RenderScene();
		pp.DrawToScreen();
		

		if (Input::GetKey(Input::Keyboard, Input::KBButtons::O)) {
			glDisable(GL_DEPTH_TEST);
			ShadowMap::__ShadowMaps__[0]->DebugDraw();
			glEnable(GL_DEPTH_TEST);
		}

		Camera::__SetRenderingCamera(nullptr);

		window.DrawGUI();
		window.SwapBuffers();
		glfwPollEvents();
	}

    // Cleanup
    for(auto i = GameObject::__objects.begin(); i < GameObject::__objects.end(); )
        i = GameObject::__objects.erase(i);
    AssetsLoader::ClearAssets();

	glfwTerminate();

	return 0;
}