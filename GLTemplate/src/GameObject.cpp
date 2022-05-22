#include "GameObject.h"
#include "Debug.h"

vector<shared_ptr<GameObject>> GameObject::__objects;

vector<shared_ptr<GameObject>> GameObject::GetComponents() { return components; }
string GameObject::GetName() { return name; }
void GameObject::SetName(string name) { this->name = name; }

void GameObject::Rotate(Vector3 rotVector)
{
	SetLocalRotation(GetLocalRotation() + rotVector);
}

Quaternion GameObject::GetLocalRotation()
{
	return rotation;
}

Quaternion GameObject::GetRotation()
{
	return GetParent() == nullptr ? GetLocalRotation() : (GetLocalRotation() + GetParent()->GetRotation());
}

void GameObject::SetLocalRotation(Vector3 rot)
{
	SetLocalRotation(Quaternion(rot));
}

void GameObject::SetLocalRotation(Quaternion q)
{
	this->rotation = q;
}

void GameObject::SetRotation(Vector3 rot)
{
	SetRotation(Quaternion(rot));
}

void GameObject::SetRotation(Quaternion rot)
{
	if (GetParent() == nullptr)
	{
		SetLocalRotation(rot);
		return;
	}

	//position = pos - GetParent()->GetPosition();
	SetLocalRotation(rot - GetParent()->GetRotation());
}

void GameObject::SetParent(GameObject* newParent)
{
	Vector3 globalPos = GetPosition();
	this->parent = newParent;

	SetPosition(globalPos);
}

Vector3 GameObject::GetPosition() 
{ 
	GameObject* parent = GetParent();
	if (parent == nullptr)
		return GetLocalPosition();

	return GetLocalPosition() * parent->GetLocalScale() + parent->GetPosition();
}
void GameObject::SetPosition(Vector3 pos)
{ 
	if (GetParent() == nullptr)
	{
		SetLocalPosition(pos);
		return;
	}

	//position = pos - GetParent()->GetPosition();
	SetLocalPosition(pos - GetParent()->GetPosition() / GetParent()->GetLocalScale());
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

Vector3 GameObject::GetScale()
{
	return GetParent() == nullptr ? GetLocalScale() : (GetLocalScale() * GetParent()->GetScale());
}

void GameObject::SetScale(Vector3 scale)
{
	if (GetParent() == nullptr)
		SetLocalScale(scale);
	else
		SetLocalScale(scale / GetParent()->GetScale());
}

Vector3 GameObject::GetLocalScale()
{
	if (!transformable)
		if (parent == nullptr)
			LOGE_E("Non-transform object needs a parent");
		else
			return parent->GetLocalScale();
	else
		return scale;
}

void GameObject::SetLocalScale(Vector3 scale)
{
	if (!transformable)
		if (parent == nullptr)
			LOGE_E("Non-transform object needs a parent");
		else
			parent->SetLocalScale(scale);
	else
		this->scale = scale;
}
