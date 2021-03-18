#include "Camera.h"
Camera* Camera::main = nullptr;
Camera* Camera::rendering = nullptr;

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	projectionMatrix = glm::perspective(glm::radians(GetFov()), aspectRatio, 0.1f, 100.0f);
}

Camera* Camera::__GetRenderingCamera()
{
	return Camera::rendering;
}

void Camera::__SetRenderingCamera(Camera* cam, float aspectRatio)
{
	Camera::rendering = cam;
	if (aspectRatio > 0 && cam != nullptr)
	{
		cam->UpdateViewMatrix();
		cam->UpdateProjectionMatrix(aspectRatio);
	}
}

Camera* Camera::GetMainCamera()
{
    return Camera::main;
}

void Camera::SetAsMainCamera()
{
	Camera::main = this;
}

float Camera::GetFov()
{
	return fov;
}

void Camera::Start()
{
	UpdateCameraVectors();
}

void Camera::SetRotation(Vector3 rot)
{
	GameObject::SetRotation(rot);

	//LOG("SetRotation");

	UpdateCameraVectors();
}

/*void Camera::Update()
{
	float velocity = speed * Time::deltaTime * (Input::GetButton("Sprint") + 1);
	Vector3 move;
	move += right * Input::GetAxis("Horizontal") * velocity;
	move += front * Input::GetAxis("Vertical") * velocity;

	Move(move);

	//LOG("Input::GetAxis(MouseX): ", Input::GetAxis("MouseX"), ", mouseY: ", Input::GetAxis("MouseY"));

	ProcessMouseMovement(Input::GetAxis("MouseX"), Input::GetAxis("MouseY"));
}


void Camera::ProcessMouseMovement(float x, float y, bool constrainPitch)
{

	x *= sensitivity;
	y *= sensitivity;

	yaw += x;
	pitch += y;

	if (constrainPitch)
	{
		if (pitch > 89)
			pitch = 89;
		if (pitch < -89)
			pitch = -89;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float offset)
{
	fov -= offset;

	if (fov < 1)
		fov = 1;
	if (fov > 180)
		fov = 180;
}*/

glm::mat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

void Camera::UpdateViewMatrix()
{
	Vector3 pos = GetPosition();

	//return glm::lookAt(pos.GetGLVector(), (pos + front).GetGLVector(), up.GetGLVector());
	viewMatrix = glm::lookAt(pos.GetGLVector(), (pos + front).GetGLVector(), worldUp.GetGLVector());
}

void Camera::UpdateCameraVectors()
{
	Vector3 rotation = GetRotation();

	front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front.y = sin(glm::radians(rotation.x));
	front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

	front.Normalize();

	right = front.Cross(worldUp).Normalized(); //glm::normalize(glm::cross(front, worldUp));
	up = right.Cross(front).Normalized(); //glm::normalize(glm::cross(right, front));
}
