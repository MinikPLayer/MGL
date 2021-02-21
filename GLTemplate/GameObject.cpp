#include "GameObject.h"

vector<GameObject*> GameObject::GetComponents() { return components; }
string GameObject::GetName() { return name; }
void GameObject::SetName(string name) { this->name = name; }
vec3 GameObject::GetPosition() 
{ 
	return GetParent() == nullptr ? position : position + GetParent()->GetPosition(); 
}
void GameObject::SetPosition(vec3 pos) 
{ 
	position = pos; 
}
