#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera
{

public:
	enum Directions
	{
		NONE,
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	float fov = 45;

	float yaw = -90;
	float pitch = 0;
	float speed = 2.5f;
	float sensitivity = 0.1f;
	float zoom = 1;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	Camera(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float yaw = 90, float pitch = 0)
		:front(glm::vec3(0,0,-1)), speed(2.5f), sensitivity(0.1f), zoom(1)
	{
		position = pos;
		this->up = up;
		this->yaw = yaw;
		this->pitch = pitch;

		UpdateCameraVectors();
	}

	float GetFov()
	{
		return fov * zoom;
	}

	void ProcessKeyboard(Directions dir, float deltaTime)
	{

		float velocity = speed * deltaTime;
		switch (dir)
		{
		case Camera::FORWARD:
			position += front * velocity;
			break;
		case Camera::BACKWARD:
			position -= front * velocity;
			break;
		case Camera::LEFT:
			position -= right * velocity;
			break;
		case Camera::RIGHT:
			position += right * velocity;
			break;
		default:
			break;
		}
	}

	void ProcessMouseMovement(float x, float y, bool constrainPitch = true)
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

	void ProcessMouseScroll(float offset)
	{
		zoom -= offset;

		if (zoom < 0.1)
			zoom = 0.1;
		if (zoom > 4)
			zoom = 4;
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

private:
	void UpdateCameraVectors()
	{
		front.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(front);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};