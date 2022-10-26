#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "GameObject.h"
#include "Input.h"
#include "TimeUtils.h"

class Camera : public GameObject
{
public:
	enum class ProjectionTypes {
		Perspective,
		Ortographic
	};

protected:
	static Camera* main;
	static Camera* rendering;

	glm::quat orientation;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	void UpdateProjectionMatrix(float aspectRatio);

	float drawDistance = 1000.f;
	float aspectRatio = 0;

	float pitch = 0;
	float yaw = 90;

	float fov = 45;

	Vector3 front;
	Vector3 up;
	Vector3 right;
	Vector3 worldUp = Vector3(0, 1, 0);

	ProjectionTypes projectionType = ProjectionTypes::Perspective;
public:
	ProjectionTypes GetProjectionType();
	void SetProjectionType(ProjectionTypes type);

	float GetAspectRatio();

	Vector2 GetCameraRotation();

	float GetDrawDistance();
	void SetDrawDistance(float distance);

	static Camera* __GetRenderingCamera();

	/// <summary>
	/// Sets rendering camera
	/// </summary>
	/// <param name="cam">Camera to be a rendering camera</param>
	/// <param name="aspectRatio">Current window aspect ratio, if set to 0 matrixes won't be updated</param>
	static void __SetRenderingCamera(Camera* cam, float aspectRatio = 0);

	static Camera* GetMainCamera();

	void LookAt(Vector3 pos);

	void SetAsMainCamera();

	float GetFov();
	void Start();
	void SetRotation(Vector3 rot);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

protected:
	void UpdateViewMatrix();
	void UpdateCameraVectors();
};