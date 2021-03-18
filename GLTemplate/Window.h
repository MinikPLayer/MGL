#pragma once

#include "Vector.h"
#include "Util.h"
#include "Input.h"
#include <string>
using namespace std;

class Window
{
	GLFWwindow* window = nullptr;
	Vector2i size;

	void UpdateSize();
public:
	enum class CursorModes
	{
		Enabled,
		Disabled,
		Hidden
	};

	void Use();
	Window(Vector2 size, string title);

	float GetAspectRatio();
	Vector2i GetSize();

	void PollEvents();
	bool ShouldClose();
	void SwapBuffers();

	void SetCursorMode(CursorModes mode);
};