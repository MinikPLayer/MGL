#include <iostream>

#include <glad/glad.h>

#include "AssetsLoader.h"
#include "Util.h"
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "Cube.h"
#include "UserScript.h"
#include "Time.h"
#include "Input.h"
#include "Vector.h"
#include "Window.h"
#include "FlybackCamera.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "lib/ImGui/imgui.h"
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::__SetMousePos(xpos, ypos);

	//camera->ProcessMouseMovement(x, y);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera->ProcessMouseScroll(yoffset);
}

void RunEvent(void (GameObject::*callback)())
{
	for (int i = 0; i < GameObject::__objects.size(); i++)
	{
		if (GameObject::__objects[i] == nullptr)
			continue;

		(GameObject::__objects[i].get()->*(callback))();
	}
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

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	//glClearColor(0, 0, 0, 1);

	

	registerBasicInput();

	LOG("Processor count: ", GetProcessorCount());


	camera = new FlybackCamera();
	camera->SetPosition(Vector3(-3, 0, 0));
	GameObject::Instantiate(camera);
	camera->SetAsMainCamera();

	for (int i = 0; i < 1; i++)
	{
		UserScript* s = new UserScript();
		s->SetPosition(Vector3(0,0,0));
		GameObject::Instantiate(s);

	}

	Sky* sky = new Sky();
	sky->SetPosition(Vector3(0, 0, 0));
	//GameObject::Instantiate(sky);

	LOG_E("Starting drawing loop...");

	bool dynamicBatching = true;
	Input::RegisterAxis(Input::Axis("DynBatch", Input::Keyboard, Input::C));

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

		if (Input::GetButtonDown("DynBatch"))
		{
			dynamicBatching = !dynamicBatching;
			LOG("Dynamic batching: ", dynamicBatching);

			if(!dynamicBatching)
				for (int i = 0; i < GameObject::__objects.size(); i++)
				{
					if (GameObject::__objects[i]->IsType<Mesh>())
					{
						Mesh* mesh = (Mesh*)GameObject::__objects[i].get();
					}
				}
		}


		glDisable(GL_DEPTH_TEST);
		sky->Update();
		sky->__Draw();
		glEnable(GL_DEPTH_TEST);

		RunEvent(&GameObject::__Draw);
		
		//sky->Update();
		//sky->__Draw();
		

		Camera::__SetRenderingCamera(nullptr);

		window.DrawGUI();
		window.SwapBuffers();
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}