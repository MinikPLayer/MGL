#include "Material.h"
#include "Util.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "Light.h"
#include "PointLight.h"

template<>
void Material::ParamT<float>::SetParam(shared_ptr<Shader>& shader)
{
	auto val = GetValue();

	Param::SetParam(shader, val.size());

	for(int i = 0;i<val.size();i++)
		shader->SetFloat(locations.get()[i], val[i]);
}

template<>
void Material::ParamT<Vector2>::SetParam(shared_ptr<Shader>& shader)
{
	auto val = GetValue();

	Param::SetParam(shader, val.size());
	for (int i = 0; i < val.size(); i++)
		shader->SetVec2(locations.get()[i], val[i].x, val[i].y);
}

template<>
void Material::ParamT<Vector3>::SetParam(shared_ptr<Shader>& shader)
{
	auto val = GetValue();

	Param::SetParam(shader, val.size());
	for (int i = 0; i < val.size(); i++)
		shader->SetVec3(locations.get()[i], val[i].x, val[i].y, val[i].z);
}

template<>
void Material::ParamT<glm::mat4>::SetParam(shared_ptr<Shader>& shader)
{
	auto val = GetValue();

	Param::SetParam(shader, val.size());
	for (int i = 0; i < val.size(); i++)
		shader->SetMat4(locations.get()[i], glm::value_ptr(val[i]));
}

template<>
void Material::ParamT<int>::SetParam(shared_ptr<Shader>& shader)
{
	auto val = GetValue();

	Param::SetParam(shader, val.size());
	for (int i = 0; i < val.size(); i++)
		shader->SetInt(locations.get()[i], val[i]);
}

template<>
void Material::ParamT<bool>::SetParam(shared_ptr<Shader>& shader)
{
	auto val = GetValue();

	Param::SetParam(shader, val.size());
	for (int i = 0; i < val.size(); i++)
		shader->SetBool(locations.get()[i], val[i]);
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

bool Material::disableTextures = false;

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

shared_ptr<Shader> Material::GetShader()
{
	return shader;
}

void Material::SetShader(shared_ptr<Shader> shader)
{
	this->shader = shader;
	DetectShaderFeatures();
}

void Material::SetTextureSlot(const char* path, int slot)
{
	textures.push_back(shared_ptr<Texture>(new Texture(path, slot)));
}

void Material::SetTextureSlot(shared_ptr<Texture> texture)
{
	for(int i = 0;i<textures.size();i++) {
		if(textures[i].get() == texture.get())
			return;
	}

	textures.push_back(texture);
}

void Material::SetMaterialTexture(shared_ptr<Texture> texture, string texName)
{
	SetTextureSlot(texture);
	SetInt("material." + texName + ".tex", texture.get()->GetSlot());
	SetBool("material." + texName + ".useTex", true);

	if(texture->GetChannelsCount() == 1)
		SetBool("material." + texName + ".useOnlyRed", true);
}

void Material::SetMaterialTexture(string texName, int slot, bool useOnlyRed)
{
	SetInt("material." + texName + ".tex", slot);
	SetBool("material." + texName + ".useTex", true);

	if(useOnlyRed)
		SetBool("material." + texName + ".useOnlyRed", true);
}

void Material::SetMaterialTexture(const char* path, string texName, int slot)
{
	auto txt = shared_ptr<Texture>(new Texture(path, slot));
	SetTextureSlot(txt);
	SetMaterialTexture(txt, texName);
}

template<class T>
class ParameterCache
{
	uint64_t lastRenderedFrame = -1;
	vector<T> cache;

	function<void(vector<T>&, Light*)> func;

public:
	vector<T> GetValue()
	{
		if (lastRenderedFrame != Time::renderedFrames)
		{
			cache.clear();

			for (int i = 0; i < Light::globalLights.size(); i++)
			{
				auto light = Light::globalLights[i];

				func(cache, light);
			}

			lastRenderedFrame = Time::renderedFrames;
		}

		return cache;
	}

	ParameterCache(function<void(vector<T>&, Light*)> f)
	{
		this->func = f;
	}
};

ParameterCache<Vector3> pointLightsPositionsCache = ParameterCache<Vector3>([](vector<Vector3>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(light->GetPosition());
});

ParameterCache<Vector3> pointLightsAmbientCache = ParameterCache<Vector3>([](vector<Vector3>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(light->GetColorVector() * 0.5f);
});

ParameterCache<Vector3> pointLightsDiffuseCache = ParameterCache<Vector3>([](vector<Vector3>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(light->GetColorVector() * 0.8f);
});

ParameterCache<Vector3> pointLightsSpecularCache = ParameterCache<Vector3>([](vector<Vector3>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(light->GetColorVector());
});

ParameterCache<float> pointLightsConstantCache = ParameterCache<float>([](vector<float>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(1.0f);
});

ParameterCache<float> pointLightsLinearCache = ParameterCache<float>([](vector<float>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(0.09f);
});

ParameterCache<float> pointLightsQuadraticCache = ParameterCache<float>([](vector<float>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(0.032f);
});

ParameterCache<float> pointLightsStrengthCache = ParameterCache<float>([](vector<float>& cache, Light* light) {
	if (light->IsType<PointLight>())
		cache.push_back(1);
});



/*static uint64_t lastRenderedFrame = -1;
static vector<Vector3> lightsPositions;
static vector<Vector3> GetLightsPositions()
{
	if (lastRenderedFrame != Time::renderedFrames)
	{
		lightsPositions.clear();
		for (int i = 0; i < Light::globalLights.size(); i++)
		{
			auto light = Light::globalLights[i];

			// For now - only point lights are supported
			if(light->IsType<PointLight>())
				lightsPositions.push_back(light->GetPosition());
		}

		lastRenderedFrame = Time::renderedFrames;
	}

	return lightsPositions;
}*/


// Used to detect shader features, like vertex position, lights, material properties, etc
// TODO: Finish implementation
void Material::DetectShaderFeatures()
{

	// Implementation not complete
	/*GLint count;
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
	}*/

	auto path = shader->GetFragmentPath();
	auto source = ReadAllLines(path.c_str());

	vector<Tuple2<string, string>> defines;
	bool started = false;
	for (int i = 0; i < source.size(); i++)
	{		
		string line = source[i];

		// Comment or empty line
		if (line.size() == 0 || StrUtils::StartsWith(line, "//"))
			continue;

		if (StrUtils::StartsWith(line, "#pragma "))
		{
			line = StrUtils::ReplaceStart(line, "#pragma ", "");
		}
		else if (StrUtils::StartsWith(line, "#define "))
		{
			line = StrUtils::ReplaceStart(line, "#define ", "");
			auto splitted = StrUtils::Split(line, ' ', 2);

			// Ignore just #define {name} without a value
			if (splitted.size() < 2)
				continue;

			defines.push_back(Tuple2<std::string, std::string>(splitted[0], splitted[1]));
		}
		else if(started) // Something else, no more shader features defines
			break;

		if (line == "[ShaderFeatures]")
		{
			started = true;
			continue;
		}

		if (!started)
			continue;

		if (line == "[ShaderFeaturesEnd]")
			break;

		for (int j = 0; j < defines.size(); j++)
		{
			line = StrUtils::Replace(line, defines[j].item1, defines[j].item2);
		}

		// Shader feature
		auto splitted = StrUtils::Split(line, '=', 2);
		if (splitted.size() < 2)
		{
			LOGE_E("Error parsing shader features at line ", (i+1));
			return;
		}

		auto split2 = StrUtils::Split(splitted[0], ':', 2);
		auto argName = split2[0];
		auto argVersion = split2[1];

		// Point Lights
		if (argName == "PointLights")
		{
			if (argVersion == "1")
			{
				auto args = StrUtils::Split(splitted[1], ',', 3);

				auto varName = args[0];
				auto maxCount = args[1];
				auto currCountVar = args[2];

				SetVec3Arr(varName, []() { return pointLightsPositionsCache.GetValue(); }, "position");
				SetVec3Arr(varName, []() { return pointLightsAmbientCache.GetValue(); }, "ambient");
				SetVec3Arr(varName, []() { return pointLightsDiffuseCache.GetValue(); }, "diffuse");
				SetVec3Arr(varName, []() { return pointLightsSpecularCache.GetValue(); }, "specular");

				SetInt(currCountVar, []() { return pointLightsPositionsCache.GetValue().size(); });

				SetFloat(varName, []() { return pointLightsConstantCache.GetValue(); }, "constant");
				SetFloat(varName, []() { return pointLightsLinearCache.GetValue(); }, "linear");
				SetFloat(varName, []() { return pointLightsQuadraticCache.GetValue(); }, "quadratic");
				SetFloat(varName, []() { return pointLightsStrengthCache.GetValue(); }, "strength");
			}
			else
			{
				LOGE_E("Unsupported PointLights shader feature version: ", argVersion);
			}
		}
	}
}

void Material::__SendToShader()
{
	if (Shader::prohibitShaderChange)
		return;

	if (!disableTextures) {
		for (int i = 0; i < textures.size(); i++) {
			textures[i]->SetActive();
		}
	}

	for (int i = 0; i < parameters.size(); i++)
	{
		parameters[i]->SetParam(shader);
	}
}

shared_ptr<Material> Material::CreatePrefabedMaterial(Material& prefab, shared_ptr<Shader> shader)
{
	Material* mat = new Material(shader);

	for (int i = 0; i < prefab.parameters.size(); i++)
		mat->parameters.push_back(prefab.parameters[i]);

	for (int i = 0; i < prefab.textures.size(); i++)
		mat->textures.push_back(prefab.textures[i]);

	return shared_ptr<Material>(mat);
}

Material::Material(shared_ptr<Shader> shader)
{
	if (shader == nullptr)
		this->shader = Shader::GetDefaultShader();
	else
		this->shader = shader;

	DetectShaderFeatures();
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
