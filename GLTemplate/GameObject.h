#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <vector>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

class GameObject
{
protected:
	vec3 position;
	string name;

	vector<GameObject*> components;
	GameObject* parent = nullptr;
public:

	GameObject* GetParent() { return parent; }

	vector<GameObject*> GetComponents();

	template<class Type>
	Type* AddComponent()
	{
		Type* t = new Type();
		t->parent = this;
		components.push_back(t);
		t->Start();

		return t;
	}

	template<class Type>
	Type* AddComponent(Type* prefab)
	{
		//Type* t = new Type();
		prefab->parent = this;
		//prefab->SetPrefab(t);
		//t->CopyValues(prefab);
		components.push_back(prefab);
		prefab->Start();

		return prefab;
	}

	string GetName();
	void SetName(string name);

	vec3 GetPosition();
	void SetPosition(vec3 pos);

	virtual void Start() {}
	virtual void Update() {}
	virtual void Draw() {}
	
};