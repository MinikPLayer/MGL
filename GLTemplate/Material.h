#pragma once

#include "Debug.h"
#include "Shader.h"
#include "Vector.h"
#include "Texture.h"
#include <vector>

class Material
{
	class Param
	{
	public:
		string name = "";
		GLint location = -1;

		size_t typeHash = -1;

		bool forceUpdateLocation;

		virtual void SetParam(Shader* shader)
		{
			if (forceUpdateLocation)
			{
				try
				{
					location = shader->GetUniformLocation(name);
				}
				catch (exception e)
				{

				}
				forceUpdateLocation = false;
			}
		}

		Param(size_t typeHash)
		{
			this->typeHash = typeHash;
			forceUpdateLocation = true;
		}
	};

	/*class ParamFloat : public Param
	{
	public:
		float value = 0;
		void SetParam(Shader* shader);

		ParamFloat() :Param(typeid(this).hash_code()) {}
	};

	class ParamVec3 : public Param
	{
	public:
		Vector3 value;
		void SetParam(Shader* shader);

		ParamVec3() :Param(typeid(this).hash_code()) {}
	};*/
	template<class Type>
	class ParamT : public Param
	{
	public:
		Type value;
		void SetParam(Shader* shader);
		
		ParamT(string name) :Param(typeid(Type).hash_code()) { this->name = name; }
		ParamT(string name, Type val) : Param(typeid(Type).hash_code()) { this->name = name; this->value = val; }
	};


	vector<Param*> parameters;
	vector<Texture> textures;

	template<class Type>
	void SetParam(string name, Type value)
	{
		size_t hash = typeid(Type).hash_code();
		for (int i = 0; i < parameters.size(); i++)
		{
			if (parameters[i]->name == name)
			{
				if (parameters[i]->typeHash != hash)
				{
					LOGE_E("Trying to set wrong type parameter");
					return;
				}

				ParamT<Type>* p = (ParamT<Type>*)(name, parameters[i]);
				p->value = value;

				return;
			}
		}

		ParamT<Type>* p = new ParamT<Type>(name);
		p->value = value;
		parameters.push_back(p);
	}

	static Material* defaultMat;
	
public:
	static Material SILVER;
	static Material RUBBER;

	static Material* GetDefaultMaterial();

	Shader* shader = nullptr;
	void __SendToShader();


	void SetFloat(string name, float value) { SetParam<float>(name, value); }
	void SetVec3(string name, Vector3 value) { SetParam<Vector3>(name, value); }
	void SetInt(string name, int value) { SetParam<int>(name, value); }
	void SetBool(string name, bool value) { SetParam<bool>(name, value); }

	void SetMaterialTexture(int slot = 0);
	void SetMaterialTexture(const char* path, int slot = 0);

	void SetTexture(const char* path, int slot);

	/*Material()
	{
		// Defaults to silver
		SetVec3("material.ambient", Vector3(0.19225, 0.19225, 0.19225));
		SetVec3("material.diffuse", Vector3(0.50754, 0.50754, 0.50754));
		SetVec3("material.specular", Vector3(0.508273, 0.508273, 0.508273));
		SetFloat("material.shininess", 0.4);
	}*/

	Material() {}
	Material(initializer_list<Param*> params);

	~Material()
	{
		for (int i = 0; i < parameters.size(); i++)
		{
			if (parameters[i] != nullptr)
				delete parameters[i];
		}
	}
};