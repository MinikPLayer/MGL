#include "GameObject.h"
#include "Debug.h"

vector<shared_ptr<GameObject>> GameObject::__objects;

vector<shared_ptr<GameObject>> GameObject::GetComponents() { return components; }
string GameObject::GetName() { return name; }
void GameObject::SetName(string name) { this->name = name; }

void GameObject::Rotate(Vector3 rotVector)
{
	SetRotation(GetRotation() + rotVector);
}

Vector3 GameObject::GetRotation()
{
	return rotation;
}

void GameObject::SetRotation(Vector3 rot)
{
	this->rotation = rot;
}

void GameObject::SetParent(GameObject* newParent)
{
	Vector3 globalPos = GetPosition();
	this->parent = newParent;

	SetPosition(globalPos);
}

Vector3 GameObject::GetPosition() 
{ 
	return GetParent() == nullptr ? GetLocalPosition() : (GetLocalPosition() + GetParent()->GetPosition());
}
void GameObject::SetPosition(Vector3 pos)
{ 
	if (GetParent() == nullptr)
	{
		SetLocalPosition(pos);
		return;
	}

	//position = pos - GetParent()->GetPosition();
	SetLocalPosition(pos - GetParent()->GetPosition());
}

void GameObject::Dispose()
{
	OnDestroy();
	//parent = nullptr;
	for (int i = 0; i < components.size(); i++)
	{
		DestroyComponent(components[i]);
	}
	parent = nullptr;
	disposed = true;
	enabled = false;
}

void GameObject::Move(Vector3 moveVector)
{
	//position += moveVector;
	SetLocalPosition(GetLocalPosition() + moveVector);
}

Vector3 GameObject::GetLocalPosition()
{
	if (!transformable)
		if(parent == nullptr)
			LOGE_E("Non-transform object needs a parent");
		else
			return parent->GetLocalPosition();
	else
		return position;
}

void GameObject::SetLocalPosition(Vector3 pos)
{
	if (!transformable)
		if (parent == nullptr)
			LOGE_E("Non-transform object needs a parent");
		else
			parent->SetLocalPosition(pos);
	else
		position = pos;
}
