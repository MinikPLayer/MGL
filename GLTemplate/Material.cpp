#include "Material.h"
#include "Util.h"
#include <glad/glad.h>

void Material::ParamT<float>::SetParam(shared_ptr<Shader>& shader)
{
	Param::SetParam(shader);

	auto val = GetValue();
	shader->SetFloat(location, val);
}

void Material::ParamT<Vector2>::SetParam(shared_ptr<Shader>& shader)
{
	Param::SetParam(shader);

	auto val = GetValue();
	shader->SetVec2(location, val.x, val.y);
}

void Material::ParamT<Vector3>::SetParam(shared_ptr<Shader>& shader)
{
	Param::SetParam(shader);

	auto val = GetValue();
	shader->SetVec3(location, val.x, val.y, val.z);
}

void Material::ParamT<int>::SetParam(shared_ptr<Shader>& shader)
{
	Param::SetParam(shader);

	auto val = GetValue();
	shader->SetInt(location, val);
}

void Material::ParamT<bool>::SetParam(shared_ptr<Shader>& shader)
{
	Param::SetParam(shader);

	auto val = GetValue();
	shader->SetBool(location, val);
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
	shared_ptr<Param>(new ParamT<Vector3>("material.diffuse.value", Vector3(0.50754, 0.50754, 0.50754))),
	shared_ptr<Param>(new ParamT<Vector3>("material.specular.value", Vector3(0.508273, 0.508273, 0.508273))),
	shared_ptr<Param>(new ParamT<float>("material.shininess", 0.4))
}, nullptr, true);
Material Material::RUBBER = Material({
	//new ParamT<Vector3>("material.ambient", Vector3(0, 0.05, 0)),
	shared_ptr<Param>(new ParamT<Vector3>("material.diffuse.value", Vector3(0.4, 0.5, 0.4))),
	shared_ptr<Param>(new ParamT<Vector3>("material.specular.value", Vector3(0.04, 0.7, 0.04))),
	shared_ptr<Param>(new ParamT<float>("material.shininess", 0.078125))
}, nullptr, true);

shared_ptr<Material> Material::defaultMat;
shared_ptr<Material> Material::GetDefaultMaterial()
{
	if (defaultMat.get() == nullptr)
	{
		defaultMat = shared_ptr<Material>(Material::CreatePrefabedMaterial(SILVER)); //new Material();
		defaultMat->shader = Shader::GetDefaultShader();
	}

	return Material::defaultMat;
}

void Material::SetTextureSlot(const char* path, int slot)
{
	textures.push_back(shared_ptr<Texture>(new Texture(path, slot)));
}

void Material::SetTextureSlot(shared_ptr<Texture> texture)
{
	textures.push_back(texture);
}

void Material::SetMaterialTexture(shared_ptr<Texture> texture, string texName)
{
	SetTextureSlot(texture);
	SetInt("material." + texName + ".tex", texture.get()->GetSlot());
	SetBool("material." + texName + ".useTex", true);
}

void Material::SetMaterialTexture(string texName, int slot)
{
	SetInt("material." + texName + ".tex", slot);
	SetBool("material." + texName + ".useTex", true);
}

void Material::SetMaterialTexture(const char* path, string texName, int slot)
{
	SetTextureSlot(path, slot);
	SetMaterialTexture(texName, slot);
}

void Material::DetectShaderFeatures()
{
	GLint count;
	GLint size;
	GLenum type;

	const GLsizei bufSize = 64;
	GLchar name[bufSize];
	GLsizei length;

	glGetProgramiv(shader->ID, GL_ACTIVE_UNIFORMS, &count);
	for (GLint i = 0; i < count; i++)
	{
		glGetActiveUniform(shader->ID, (GLuint)i, bufSize, &length, &size, &type, name);

		LOG("Uniform id ", i, " Type: ", type, " Name: ", name);
	}
}

void Material::__SendToShader()
{
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->SetActive();
	}

	for (int i = 0; i < parameters.size(); i++)
	{
		parameters[i]->SetParam(shader);
	}
}

Material::Material(initializer_list<shared_ptr<Param>> params, shared_ptr<Shader> shader, bool skipShaderInit)
{
	if (!skipShaderInit)
	{
		if (shader == nullptr)
			this->shader = Shader::GetDefaultShader();
		else
			this->shader = shader;

		DetectShaderFeatures();
	}

	for (auto param : params)
	{
		//parameters.push_back(param);
		parameters.push_back(param);
	}
}
