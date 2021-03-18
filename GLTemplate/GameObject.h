#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <vector>
#include "Debug.h"
#include "Vector.h"
#include <typeinfo>
#include "Util.h"
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

class GameObject
{
protected:
	Vector3 position;
	Vector3 rotation;
	string name;

	vector<GameObject*> components;
	GameObject* parent = nullptr;

	size_t typeHash = 0;

	
	bool transform;
public:
	GameObject() { transform = true; }
	GameObject(GameObject* parent, bool transform = true) {
		parent->AddComponent(this);
		this->transform = transform;
	}

	static vector<GameObject*> __objects;


	template<class Type>
	bool IsType();

	template<class Type>
	void __SetType();

	/// <summary>
	/// If false object won't receive any events ( Start, Update, etc )
	/// </summary>
	bool enabled = true;

	void SetParent(GameObject* parent);
	GameObject* GetParent() { return parent; }

	vector<GameObject*> GetComponents();

	template<class Type>
	static Type* Instantiate()
	{
		Type* t = new Type();
		//t->typeHash = typeid(Type).hash_code();
		t->__SetType<Type>();
		__objects.push_back(t);
		t->Start();
		return t;
	}

	template<class Type>
	static Type* Instantiate(Type* prefab)
	{
		//Type* t = new Type();
		//prefab->SetPrefab(t);
		//t->CopyValues(prefab);
		//prefab->typeHash = typeid(Type).hash_code();
		prefab->__SetType<Type>();
		__objects.push_back(prefab);

		prefab->Start();
		return prefab;
	}

	template<class Type>
	void Destroy(Type*& instance)
	{
		if (instance == nullptr)
		{
			LOGW_E("Trying to destroy null object!");
			return;
		}

		if (instance->GetParent() != nullptr)
		{
			instance->GetParent()->DestroyComponent(instance);
			instance = nullptr;
			return;
		}
		else
		{
			for (int i = 0; i < __objects.size(); i++)
			{
				if (__objects[i] == instance)
				{
					__objects[i]->Dispose();
					delete __objects[i];
					__objects[i] == nullptr;
					instance = nullptr;
					return;
				}
			}
		}

		LOGW_E("Trying to destroy instance not present in objects list!");
	}


	template<class Type>
	void DestroyComponent(Type*& instance)
	{
		if (instance == nullptr)
		{
			LOGW_E("Trying to destroy null instance");
			return;
		}

		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == instance)
			{
				components[i]->Dispose();
				delete components[i];
				components[i] = nullptr;
				instance = nullptr;
				return;
			}
		}

		LOGW_E("Trying to destroy instance not present in components list!");
	}

	bool disposed = false;
	/// <summary>
	/// Function to dispose and clean object when destroying
	/// </summary>
	void Dispose();

	template<class Type>
	Type* AddComponent()
	{
		/*Type* t = new Type();
		t->parent = this;
		typeHash = typeid(Type).hash_code();
		//t->typeInfo = typeid(Type);
		components.push_back(t);
		t->Start();

		return t;*/
		Type* t = Instantiate<Type>();
		t->parent = this;
		return t;
	}

	template<class Type>
	Type* AddComponent(Type* prefab)
	{
		//Type* t = new Type();
		prefab->parent = this;
		/*typeHash = typeid(Type).hash_code();
		//prefab->typeInfo = typeid(Type);
		//prefab->SetPrefab(t);
		//t->CopyValues(prefab);
		components.push_back(prefab);
		prefab->Start();

		return prefab;*/

		return Instantiate<Type>(prefab);
	}

	string GetName();
	void SetName(string name);

	void Move(Vector3 moveVector);

	void Rotate(Vector3 rotVector);
	Vector3 GetRotation();
	virtual void SetRotation(Vector3 rot);

	Vector3 GetPosition();
	virtual void SetPosition(Vector3 pos);

	Vector3 GetLocalPosition();
	virtual void SetLocalPosition(Vector3 pos);

	virtual void Start() {}
	virtual void Update() {}
	virtual void OnDestroy() {}
	

	virtual void __Draw() {}
};

template<class Type>
inline bool GameObject::IsType()
{
	//LOG("Type hash: ", typeid(Type).hash_code(), ", This hash: ", typeHash);
	return typeid(Type).hash_code() == typeHash;
}

template<class Type>
inline void GameObject::__SetType()
{
	typeHash = typeid(Type).hash_code();

	//LOG("TypeHash: ", typeHash);
}

