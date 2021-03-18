#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "GameObject.h"
#include "Input.h"
#include "Time.h"

class Camera : public GameObject
{

	static Camera* main;
	static Camera* rendering;

protected:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	void UpdateProjectionMatrix(float aspectRatio);


	float fov = 45;

	Vector3 front;
	Vector3 up;
	Vector3 right;
	Vector3 worldUp = Vector3(0, 1, 0);
public:
	static Camera* __GetRenderingCamera();

	/// <summary>
	/// Sets rendering camera
	/// </summary>
	/// <param name="cam">Camera to be a rendering camera</param>
	/// <param name="aspectRatio">Current window aspect ratio, if set to 0 matrixes won't be updated</param>
	static void __SetRenderingCamera(Camera* cam, float aspectRatio = 0);

	static Camera* GetMainCamera();


	/*Camera(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float yaw = 90, float pitch = 0)
		:front(glm::vec3(0,0,-1)), speed(2.5f), sensitivity(0.1f), zoom(1)
	{
		position = pos;
		this->up = up;
		this->yaw = yaw;
		this->pitch = pitch;


	}*/

	void SetAsMainCamera();

	float GetFov();
	//void Update();
	void Start();
	void SetRotation(Vector3 rot);
	//void ProcessMouseMovement(float x, float y, bool constrainPitch = true);
	//void ProcessMouseScroll(float offset);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

protected:
	void UpdateViewMatrix();
	void UpdateCameraVectors();
};