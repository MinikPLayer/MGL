#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Vector.h"
#include "Util.h"
#include "Input.h"
#include <string>
using namespace std;

class Window
{
	static Window* mainWindow;

	GLFWwindow* window = nullptr;
	Vector2i size;

public:
	void UpdateSize();
	enum class CursorModes
	{
		Enabled,
		Disabled,
		Hidden
	};

	static Window* GetMainWindow();

	void Use();
	Window(Vector2 size, string title, bool main);
	~Window();

	float GetAspectRatio();
	Vector2i GetSize();

	void SetViewport();
	void SetViewport(int x, int y, int width, int height);
	void DrawGUI();

	void PollEvents();
	bool ShouldClose();
	void SwapBuffers();

	void SetCursorMode(CursorModes mode);
};