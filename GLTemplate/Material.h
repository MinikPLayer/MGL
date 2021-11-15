#pragma once

#include "Asset.h"
#include "Debug.h"
#include "Shader.h"
#include "Vector.h"
#include "Texture.h"
#include <vector>
#include <functional>

class Material
{
	class Param
	{
		int lastSize = -1;

	protected:
		void SetParam(shared_ptr<Shader>& shader, int size)
		{
			if (forceUpdateLocation || (isArray && size != lastSize))
			{
				try
				{
					//location = shader->GetUniformLocation(name);
					locations.reset(new GLint[size]);
					for (int i = 0; i < size; i++)
					{
						string paramName = isArray ? name + "[" + to_string(i) + "]" : name;
						if (subName.size() != 0)
							paramName += "." + subName;
						locations.get()[i] = shader->GetUniformLocation(paramName);
					}
				}
				catch (exception e)
				{
					LOGE_E("Error getting uniform location for ", name);
				}
				forceUpdateLocation = false;
				lastSize = size;
			}
		}

	public:
		string name = "";
		string subName = "";
		unique_ptr<GLint> locations = nullptr;

		size_t typeHash = -1;

		bool forceUpdateLocation;
		bool isArray = false;

		virtual void SetParam(shared_ptr<Shader>& shader)
		{
			LOGW_E("Param ", name, " not implemented!");
		}

		Param(size_t typeHash)
		{
			this->typeHash = typeHash;
			forceUpdateLocation = true;
		}
	};

	template<class Type>
	class ParamT : public Param
	{
	public:
		vector<Type> value;
		function<vector<Type>()> arrayFuncPtr = nullptr;
		function<Type()> funcPtr = nullptr;

		//Type value;
		//function<Type()> funcPtr = nullptr;



		vector<Type> GetValue() 
		{ 
			if (arrayFuncPtr != nullptr)
			{
				value = arrayFuncPtr();
				isArray = true;
			}
			else if (funcPtr != nullptr)
			{
				value.clear();
				value.push_back(funcPtr());
			}
			return value; 
		}

		void SetParam(shared_ptr<Shader>& shader);
		
		ParamT(string name) :Param(typeid(Type).hash_code()) { this->name = name; }
		ParamT(string name, Type val) : Param(typeid(Type).hash_code()) { this->name = name; this->value.push_back(val); }
		ParamT(string name, function<Type()> funcPtr) : Param(typeid(Type).hash_code()) { this->name = name; this->funcPtr = funcPtr; }
	};


	vector<shared_ptr<Param>> parameters;
	vector<shared_ptr<Texture>> textures;

	template<class Type>
	void SetParam(string name, Type value, function<Type()> funcPtr = nullptr, function<vector<Type>()> arrayFuncPtr = nullptr, string subName = "")
	{
		size_t hash = typeid(Type).hash_code();
		for (int i = 0; i < parameters.size(); i++)
		{
			if (parameters[i].get()->name == name && parameters[i].get()->subName == subName)
			{
				if (parameters[i].get()->typeHash != hash)
				{
					LOGE_E("Trying to set wrong type parameter");
					return;
				}

				ParamT<Type>* p = (ParamT<Type>*)(name, parameters[i].get());
				p->value.push_back(value);
				p->funcPtr = funcPtr;
				p->arrayFuncPtr = arrayFuncPtr;
				p->subName = subName;

				return;
			}
		}

		ParamT<Type>* p = new ParamT<Type>(name);
		p->value.push_back(value);
		p->funcPtr = funcPtr;
		p->arrayFuncPtr = arrayFuncPtr;
		p->subName = subName;
		parameters.push_back(std::shared_ptr<ParamT<Type>>(p));
	}

	static shared_ptr<Material> defaultMat;

	shared_ptr<Shader> shader;
public:
	static bool disableTextures;

	// DO NOT USE DIRECTLY, use with CreatePrefabedMaterial(Material::SILVER)	
	static Material SILVER;
	// DO NOT USE DIRECTLY, use with CreatePrefabedMaterial(Material::RUBBER)	
	static Material RUBBER;

	static shared_ptr<Material> GetDefaultMaterial();

	shared_ptr<Shader> GetShader();
	void SetShader(shared_ptr<Shader> shader);

	void __SendToShader();


	void SetFloat(string name, float value, string subName = "") { SetParam<float>(name, value, nullptr, nullptr, subName); }
	void SetFloat(string name, function<float()> func, string subName = "") { SetParam<float>(name, 0, func, nullptr, subName); }
	void SetFloat(string name, function<vector<float>()> func, string subName = "") { SetParam<float>(name, 0, nullptr, func, subName); }
	void SetVec2(string name, Vector2 value) { SetParam<Vector2>(name, value); }
	void SetVec2(string name, function<Vector2()> func) { SetParam<Vector2>(name, Vector2(0,0), func); }
	void SetVec3(string name, Vector3 value) { SetParam<Vector3>(name, value); }
	void SetVec3(string name, function<Vector3()> func) { SetParam<Vector3>(name, Vector3(0, 0, 0), func); }
	void SetVec3Arr(string name, function<vector<Vector3>()> func, string subName = "") { SetParam<Vector3>(name, Vector3(0, 0, 0), nullptr, func, subName); }
	void SetMat4(string name, function<glm::mat4()> func) { SetParam<glm::mat4>(name, glm::mat4(1), func); }
	void SetInt(string name, int value) { SetParam<int>(name, value); }
	void SetInt(string name, function<int()> func, string subName = "") { SetParam<int>(name, 0, func, nullptr, subName); }
	void SetBool(string name, bool value) { SetParam<bool>(name, value); }

	/// <summary>
	/// Set material texture
	/// </summary>
	/// <param name="texName">Shader texture name, eg diffuse, specular</param>
	/// <param name="slot">Texture slot</param>
	void SetMaterialTexture(shared_ptr<Texture> texture, string texName = "diffuse");
	void SetMaterialTexture(string texName = "diffuse", int slot = 0, bool useOnlyRed = false);
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

	void DetectShaderFeatures();

	/// <summary>
	/// Creates functional prefabed material
	/// </summary>
	/// <param name="mat">Prefabed material, eg SILVER or RUBBER</param>
	/// <returns>Created material</returns>
	static shared_ptr<Material> CreatePrefabedMaterial(Material& prefab, shared_ptr<Shader> shader = nullptr);

	Material(shared_ptr<Shader> shader = nullptr);

	Material(initializer_list<shared_ptr<Param>> params, shared_ptr<Shader> shader = nullptr, bool skipShaderInit = false);
};