#include "Cameras/FlybackCamera.h"

void FlybackCamera::Start()
{
	Camera::Start();
	LOG("FlyBackCamera start!");
}

void FlybackCamera::Update()
{
	Camera::Update();

	float velocity = speed * Time::deltaTime * (Input::GetButton("Sprint") + 1);
	Vector3 move(0, 0, 0);
	

	move += right * Input::GetAxis("Horizontal") * velocity;
	move += front * Input::GetAxis("Vertical") * velocity;

	Move(move);
	ProcessMouseMovement(Input::GetAxis("MouseX"), Input::GetAxis("MouseY"));
}

void FlybackCamera::ProcessMouseMovement(float x, float y, bool constrainPitch)
{

	x *= sensitivity;
	y *= sensitivity;

	yaw += x;
	pitch += y;
	UpdateCameraVectors();
}

void FlybackCamera::ProcessMouseScroll(float offset)
{
	fov -= offset;

	if (fov < 1)
		fov = 1;
	if (fov > 180)
		fov = 180;
}
