#include "Window.h"

void Window::UpdateSize()
{
	glViewport(0, 0, size.x, size.y);
}

void Window::Use()
{
	glfwMakeContextCurrent(window);
}

Window::Window(Vector2 size, string title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->size = size;
	LOG_E("Window size: ", size.x, ", ", size.y);

	window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
		LOGF_E("Cannot create GLFW window");
		return;
	}

	Use();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOGF_E("Failed to initialize GLAD");
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	SetViewport();
}

float Window::GetAspectRatio()
{
	if (size.y == 0) return 1;

	return (float)size.x / (float)size.y;
}

Vector2i Window::GetSize()
{
	return size;
}

void Window::SetViewport() { SetViewport(0, 0, size.x, size.y); }
void Window::SetViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }


void Window::PollEvents()
{
	//glfwPollEvents();

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	//if(size != Vector2i(w,h))
	if (size != Vector2i(w, h))
	{
		size = Vector2i(w, h);
		UpdateSize();
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	Input::__SetMousePos(x, y);

	Input::__Update(window);


	if (Input::GetButtonDown("CloseWindow"))
		glfwSetWindowShouldClose(window, true);

	
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::SetCursorMode(CursorModes mode)
{
	int m = 0;
	switch (mode)
	{
	case Window::CursorModes::Enabled:
		m = GLFW_CURSOR_NORMAL;
		break;
	case Window::CursorModes::Disabled:
		m = GLFW_CURSOR_DISABLED;
		break;
	case Window::CursorModes::Hidden:
		m = GLFW_CURSOR_HIDDEN;
		break;
	default:
		break;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
