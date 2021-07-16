#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <vector>
#include "Debug.h"
#include "Vector.h"
#include <typeinfo>
#include <memory>
#include "Util.h"
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

class GameObject
{
protected:
	Vector3 position = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);
	string name;

	// Object child components
	vector<shared_ptr<GameObject>> components;

	// Object parent
	GameObject* parent = nullptr;

	// Type hash, used to check if object is a certain type
	size_t typeHash = 0;

	// True for moveable gameobjects, false for components without any position / rotation
	bool transformable;
public:
	GameObject() { transformable = true; }

	GameObject(GameObject* parent, bool transformable = true) {
		parent->AddComponent(this);
		this->transformable = transformable;
	}

	/// <summary>
	/// [ENGINE] List of spawned objects 
	/// </summary>
	static vector<shared_ptr<GameObject>> __objects;


	/// <summary>
	/// Checks if this object is of specified type
	/// </summary>
	/// <typeparam name="Type">Type to compare</typeparam>
	/// <returns>True if object is an instance of specified type, false otherwise</returns>
	template<class Type>
	bool IsType();

	/// <summary>
	/// [ENGINE] Sets object type
	/// </summary>
	/// <typeparam name="Type">Object type</typeparam>
	template<class Type>
	void __SetType();

	/// <summary>
	/// If false object won't receive any events ( Start, Update, etc )
	/// </summary>
	bool enabled = true;

	void SetParent(GameObject* parent);
	GameObject* GetParent() { return parent; }

	vector<shared_ptr<GameObject>> GetComponents();

	/// <summary>
	/// Instantiate object using it's default constructor
	/// </summary>
	/// <typeparam name="Type">Object type</typeparam>
	/// <returns>Shared pointer to the spawned object</returns>
	template<class Type>
	static shared_ptr<Type> Instantiate()
	{
		Type* t = new Type();
		shared_ptr<Type> obj(t);
		//t->typeHash = typeid(Type).hash_code();
		t->__SetType<Type>();
		__objects.push_back(obj);
		t->Start();
		return obj;
	}

	/// <summary>
	/// Instantiate object using a shared pointer to the prefab
	/// </summary>
	/// <typeparam name="Type">Object type</typeparam>
	/// <param name="prefab">Shared pointer to the prefab</param>
	/// <returns>Shared pointer to the spawned object</returns>
	template<class Type>
	static shared_ptr<Type> Instantiate(shared_ptr<Type> prefab)
	{
		Type* ptr = prefab.get();
		if (ptr == nullptr)
		{
			LOGF_E("Trying to instantiate null prefab");
			return nullptr;
		}

		prefab.get()->__SetType<Type>();
		__objects.push_back(prefab);

		prefab.get()->Start();
		return prefab;
	}

	/// <summary>
	/// Instantiate object using a pointer to the prefab
	/// </summary>
	/// <typeparam name="Type">Object type</typeparam>
	/// <param name="prefab">Pointer to the prefab</param>
	/// <returns>Shared pointer to the spawned object</returns>
	template<class Type>
	static shared_ptr<Type> Instantiate(Type* prefab)
	{
		prefab->__SetType<Type>();
		shared_ptr<Type> ptr(prefab);
		__objects.push_back(ptr);

		prefab->Start();
		return ptr;
	}

	template<class Type>
	void Destroy(shared_ptr<Type>& instance)
	{
		Type* ptr = instance.get();

		if (ptr == nullptr)
		{
			LOGW_E("Trying to destroy null object!");
			return;
		}

		if (ptr->GetParent() != nullptr)
		{
			ptr->GetParent()->DestroyComponent(instance);
			instance.reset();
			return;
		}
		else
		{
			for (int i = 0; i < __objects.size(); i++)
			{
				if (__objects[i] == instance)
				{
					// Clean and mark as disposed
					__objects[i]->Dispose();
					return;
				}
			}
		}

		LOGW_E("Trying to destroy instance not present in objects list!");
	}

	template<class Type>
	void DestroyComponent(shared_ptr<Type>& instance)
	{
		if (instance.get() == nullptr)
		{
			LOGW_E("Trying to destroy null instance");
			return;
		}

		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == instance)
			{
				// Clean and mark as disposed
				components[i]->Dispose();

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

	/// <summary>
	/// Finds and returns shared pointer of this object (slow!)
	/// </summary>
	/// <returns>Shared pointer to this object, nullptr shared pointer if not found</returns>
	shared_ptr<GameObject> GetSharedPointer()
	{
		return GetSharedPointer<GameObject>();
	}

	/// <summary>
	/// Finds and returns shared pointer of this object (slow!)
	/// </summary>
	/// <typeparam name="Type">Shared pointer type</typeparam>
	/// <returns>Shared pointer to this object, nullptr shared pointer if not found</returns>
	template <class Type>
	shared_ptr<Type> GetSharedPointer()
	{
		for (int i = 0; i < __objects.size(); i++)
		{
			if (__objects[i].get() == this)
				return __objects[i];
		}

		LOGF_E("Cannot find shared pointer reference!");
		return shared_ptr<Type>();
	}

	/// <summary>
	/// Add component using it's default constructor
	/// </summary>
	/// <typeparam name="Type">Component type</typeparam>
	/// <returns>Shared pointer to spawned component</returns>
	template <class Type>
	shared_ptr<Type> AddComponent()
	{
		shared_ptr<Type> t = Instantiate<Type>();
		t.get()->parent = this;
		components.push_back(t);
		return t;
	}

	/// <summary>
	/// Add component using shared pointer prefab
	/// </summary>
	/// <typeparam name="Type">Prefab type</typeparam>
	/// <param name="prefab">Component prefab</param>
	/// <returns>Shared pointer to spawned component</returns>
	template <class Type>
	shared_ptr<Type> AddComponent(shared_ptr<Type> prefab)
	{
		prefab.get()->parent = this;
		shared_ptr<Type> instance = Instantiate<Type>(prefab);
		components.push_back(instance);

		return instance;
	}

	/// <summary>
	/// Add component using pointer prefab
	/// </summary>
	/// <typeparam name="Type">Component type</typeparam>
	/// <param name="prefab">Component prefab</param>
	/// <returns>Shared pointer to spawned component</returns>
	template <class Type>
	shared_ptr<Type> AddComponent(Type* prefab)
	{
		prefab->parent = this;
		shared_ptr<Type> instance = Instantiate<Type>(prefab);
		components.push_back(instance);

		return instance;
	}

	string GetName();
	void SetName(string name);

	/// <summary>
	/// Move object
	/// </summary>
	/// <param name="moveVector">Move vector</param>
	void Move(Vector3 moveVector);

	/// <summary>
	/// Rotate object
	/// </summary>
	/// <param name="rotVector">Rotate vector</param>
	void Rotate(Vector3 rotVector);
	Vector3 GetRotation();
	virtual void SetRotation(Vector3 rot);

	/// <summary>
	/// Get object world position
	/// </summary>
	/// <returns>World position</returns>
	Vector3 GetPosition();
	/// <summary>
	/// Set object world position
	/// </summary>
	/// <param name="pos">New world position</param>
	virtual void SetPosition(Vector3 pos);

	/// <summary>
	/// Get object local position relative to it's parent
	/// </summary>
	/// <returns>Local position</returns>
	Vector3 GetLocalPosition();
	/// <summary>
	/// Set object local position, relative to it's parent
	/// </summary>
	/// <param name="pos">New local position</param>
	virtual void SetLocalPosition(Vector3 pos);

	Vector3 GetScale();
	virtual void SetScale(Vector3 scale);

	Vector3 GetLocalScale();
	virtual void SetLocalScale(Vector3 scale);

	/// <summary>
	/// Event fired when instantiated
	/// </summary>
	virtual void Start() {}
	/// <summary>
	/// Event fired every frame
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// Event fired when destroyed
	/// </summary>
	virtual void OnDestroy() {}
	

	/// <summary>
	/// [ENGINE] Event used to draw object
	/// </summary>
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

