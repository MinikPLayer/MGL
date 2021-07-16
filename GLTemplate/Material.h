#pragma once

#include "Asset.h"
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

		virtual void SetParam(shared_ptr<Shader>& shader)
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
		void SetParam(shared_ptr<Shader>& shader);
		
		ParamT(string name) :Param(typeid(Type).hash_code()) { this->name = name; }
		ParamT(string name, Type val) : Param(typeid(Type).hash_code()) { this->name = name; this->value = val; }
	};


	vector<shared_ptr<Param>> parameters;
	vector<shared_ptr<Texture>> textures;

	template<class Type>
	void SetParam(string name, Type value)
	{
		size_t hash = typeid(Type).hash_code();
		for (int i = 0; i < parameters.size(); i++)
		{
			if (parameters[i].get()->name == name)
			{
				if (parameters[i].get()->typeHash != hash)
				{
					LOGE_E("Trying to set wrong type parameter");
					return;
				}

				ParamT<Type>* p = (ParamT<Type>*)(name, parameters[i].get());
				p->value = value;

				return;
			}
		}

		ParamT<Type>* p = new ParamT<Type>(name);
		p->value = value;
		parameters.push_back(std::shared_ptr<ParamT<Type>>(p));
	}

	static shared_ptr<Material> defaultMat;
	
public:
	// DO NOT USE DIRECTLY, use with CreatePrefabedMaterial(Material::SILVER)	
	static Material SILVER;
	// DO NOT USE DIRECTLY, use with CreatePrefabedMaterial(Material::RUBBER)	
	static Material RUBBER;

	static shared_ptr<Material> GetDefaultMaterial();

	shared_ptr<Shader> shader;
	void __SendToShader();


	void SetFloat(string name, float value) { SetParam<float>(name, value); }
	void SetVec2(string name, Vector2 value) { SetParam<Vector2>(name, value); }
	void SetVec3(string name, Vector3 value) { SetParam<Vector3>(name, value); }
	void SetInt(string name, int value) { SetParam<int>(name, value); }
	void SetBool(string name, bool value) 
	{ 
		SetParam<bool>(name, value); 
	}

	/// <summary>
	/// Set material texture
	/// </summary>
	/// <param name="texName">Shader texture name, eg diffuse, specular</param>
	/// <param name="slot">Texture slot</param>
	void SetMaterialTexture(shared_ptr<Texture> texture, string texName = "diffuse");
	void SetMaterialTexture(string texName = "diffuse", int slot = 0);
	void SetMaterialTexture(const char* path, string texName = "diffuse", int slot = 0);

	
	/// <summary>
	/// Sets texture slot
	/// </summary>
	void SetTextureSlot(shared_ptr<Texture> texture);
	/// <summary>
	/// Sets texture slot
	/// </summary>
	void SetTextureSlot(const char* path, int slot);

	/*Material()
	{
		// Defaults to silver
		SetVec3("material.ambient", Vector3(0.19225, 0.19225, 0.19225));
		SetVec3("material.diffuse", Vector3(0.50754, 0.50754, 0.50754));
		SetVec3("material.specular", Vector3(0.508273, 0.508273, 0.508273));
		SetFloat("material.shininess", 0.4);
	}*/

	/// <summary>
	/// Creates functional prefabed material
	/// </summary>
	/// <param name="mat">Prefabed material, eg SILVER or RUBBER</param>
	/// <returns>Created material</returns>
	static shared_ptr<Material> CreatePrefabedMaterial(Material& prefab, shared_ptr<Shader> shader = nullptr)
	{
		Material* mat = new Material(prefab);
		if (shader == nullptr)
			mat->shader = Shader::GetDefaultShader();
		else
			mat->shader = shader;

		return shared_ptr<Material>(mat);		
	}

	Material(shared_ptr<Shader> shader = nullptr) 
	{
		if (shader == nullptr)
			this->shader = Shader::GetDefaultShader();
		else
			this->shader = shader;
	}

	Material(initializer_list<shared_ptr<Param>> params, shared_ptr<Shader> shader = nullptr, bool skipShaderInit = false);
};