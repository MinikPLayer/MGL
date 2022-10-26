#include "Camera.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

Camera* Camera::main = nullptr;
Camera* Camera::rendering = nullptr;

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	const float near_plane = 0.1f;
	if(projectionType == Camera::ProjectionTypes::Perspective)
		projectionMatrix = glm::perspective(glm::radians(GetFov()), aspectRatio, 0.1f, drawDistance);
	else
	{
		auto fov = GetFov() / 2.0f;
		projectionMatrix = glm::ortho(-fov, fov, -fov, fov, 0.1f, drawDistance);
	}
	this->aspectRatio = aspectRatio;
}

float Camera::GetAspectRatio()
{
	return aspectRatio;
}

Vector2 Camera::GetCameraRotation()
{
	return Vector2(pitch, yaw);
}

float Camera::GetDrawDistance()
{
	return drawDistance;
}

void Camera::SetDrawDistance(float distance)
{
	drawDistance = distance;
	UpdateProjectionMatrix(aspectRatio);
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

void Camera::LookAt(Vector3 pos)
{
	orientation = glm::lookAt(GetPosition().GetGLVector(), pos.GetGLVector(), glm::vec3(0, 1, 0));
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
	UpdateCameraVectors();
}

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
	glm::quat qPitch = glm::angleAxis(-glm::radians(pitch), glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));

	//For a FPS camera we can omit roll
	orientation = qPitch * qYaw;
	orientation = glm::normalize(orientation);
	glm::mat4 rotate = glm::mat4_cast(orientation);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -GetPosition().GetGLVector());

	viewMatrix = rotate * translate;
}

void Camera::UpdateCameraVectors()
{
	UpdateViewMatrix();

	front = glm::rotate(glm::inverse(orientation), glm::vec3(0.0, 0.0, -1.0));
	right = glm::rotate(glm::inverse(orientation), glm::vec3(1.0, 0.0, 0.0));
	up = glm::vec3(0.0, 1.0, 0.0);
}

Camera::ProjectionTypes Camera::GetProjectionType() {
	return projectionType;
}

void Camera::SetProjectionType(Camera::ProjectionTypes type) {
	this->projectionType = type;
	UpdateProjectionMatrix(aspectRatio);
}
