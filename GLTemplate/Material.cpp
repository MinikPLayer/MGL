#include "Material.h"
#include "Util.h"
#include <glad/glad.h>

void Material::ParamT<float>::SetParam(Shader* shader)
{
	Param::SetParam(shader);

	shader->SetFloat(location, value);
}

void Material::ParamT<Vector3>::SetParam(Shader* shader)
{
	Param::SetParam(shader);

	shader->SetVec3(location, value.x, value.y, value.z);
}

void Material::ParamT<int>::SetParam(Shader* shader)
{
	Param::SetParam(shader);

	shader->SetInt(location, value);
}

void Material::ParamT<bool>::SetParam(Shader* shader)
{
	Param::SetParam(shader);

	shader->SetBool(location, value);
}

/*void Material::SetFloat(string name, float value)
{
	ParamT<float> p;
	p.name = name;
	p.value = value;

	parameters.push_back(p);
}

void Material::SetVec3(string name, Vector3 value)
{
	ParamT<Vector3> p;
	p.name = name;
	p.value = value;

	parameters.push_back(p);
}*/

Material Material::SILVER = Material({
	//new ParamT<Vector3>("material.ambient", Vector3(0.19225, 0.19225, 0.19225)),
	//new ParamT<Vector3>("material.diffuse", Vector3(0.50754, 0.50754, 0.50754)),
	new ParamT<int>("material.diffuse", 0), // texture
	new ParamT<Vector3>("material.specular", Vector3(0.508273, 0.508273, 0.508273)),
	new ParamT<float>("material.shininess", 0.4)
});
Material Material::RUBBER = Material({
	//new ParamT<Vector3>("material.ambient", Vector3(0, 0.05, 0)),
	//new ParamT<Vector3>("material.diffuse", Vector3(0.4, 0.5, 0.4)),
	new ParamT<int>("material.diffuse", 0),
	new ParamT<Vector3>("material.specular", Vector3(0.04, 0.7, 0.04)),
	new ParamT<float>("material.shininess", 0.078125)
});

Material* Material::defaultMat = nullptr;
Material* Material::GetDefaultMaterial()
{
	if (defaultMat == nullptr)
	{
		defaultMat = &SILVER; //new Material();
		defaultMat->shader = Shader::GetDefaultShader();
	}

	return Material::defaultMat;
}

void Material::SetTexture(const char* path, int slot)
{
	textures.push_back(Texture(path, slot));
}

void Material::SetMaterialTexture(int slot)
{
	SetInt("material.diffuse", slot);
	SetBool("material.useTexture", false);
}

void Material::SetMaterialTexture(const char* path, int slot)
{
	SetTexture(path, slot);
	SetMaterialTexture(slot);
}

void Material::__SendToShader()
{
	for (int i = 0; i < textures.size(); i++)
	{
		//textures[i].SetActive();
	}

	for (int i = 0; i < parameters.size(); i++)
	{
		parameters[i]->SetParam(shader);
	}
}

Material::Material(initializer_list<Param*> params)
{
	for (auto param : params)
	{
		parameters.push_back(param);
	}
}
