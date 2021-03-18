#include "FlybackCamera.h"

void FlybackCamera::Start()
{
	Camera::Start();
	LOG("FlyBackCamera start!");
}

void FlybackCamera::Update()
{
	Camera::Update();

	float velocity = speed * Time::deltaTime * (Input::GetButton("Sprint") + 1);
	Vector3 move;
	move += right * Input::GetAxis("Horizontal") * velocity;
	move += front * Input::GetAxis("Vertical") * velocity;

	Move(move);

	//LOG("Input::GetAxis(MouseX): ", Input::GetAxis("MouseX"), ", mouseY: ", Input::GetAxis("MouseY"));

	ProcessMouseMovement(Input::GetAxis("MouseX"), Input::GetAxis("MouseY"));
}

void FlybackCamera::ProcessMouseMovement(float x, float y, bool constrainPitch)
{

	x *= sensitivity;
	y *= sensitivity;

	Vector3 rot = GetRotation();

	//yaw += x;
	//pitch += y;
	rot.y += x;
	rot.x += y;


	if (constrainPitch)
	{
		if (rot.x > 89)
			rot.x = 89;
		if (rot.x < -89)
			rot.x = -89;
	}

	// UpdateCameraVectors is already in SetRotation ( in base Camera class )
	SetRotation(rot);
	//UpdateCameraVectors();
}

void FlybackCamera::ProcessMouseScroll(float offset)
{
	fov -= offset;

	if (fov < 1)
		fov = 1;
	if (fov > 180)
		fov = 180;
}
