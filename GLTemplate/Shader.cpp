#include "Shader.h"
#include "AssetsLoader.h"

#include <glm/gtc/type_ptr.hpp>
#include "Util.h"
#include "Camera.h"

const string Shader::TYPE_FRAGMENT = "fragment";
const string Shader::TYPE_VERTEX = "vertex";

void Shader::UpdateUniformsLocations()
{
	modelLocation = glGetUniformLocation(ID, "model");
	viewLocation = glGetUniformLocation(ID, "view");
	projectionLocation = glGetUniformLocation(ID, "projection");
}

Shader::Shader(string vertexPath, string fragmentPath)
{
	loaded = LoadShaders(vertexPath, fragmentPath);
}

bool Shader::LoadShaders(string vertexPath, string fragmentPath)
{
	// Init some variables
	GLuint vertex = 0;
	GLuint fragment = 0;

	// Try to compile vertex shader
	if (!CompileShader(vertexPath, GL_VERTEX_SHADER, vertex))
	{
		throw ShaderLoadException(TYPE_VERTEX, "");
		return false;
	}

	// Try to compile fragment shader
	if (!CompileShader(fragmentPath, GL_FRAGMENT_SHADER, fragment))
	{
		glDeleteShader(vertex);
		throw ShaderLoadException(TYPE_FRAGMENT, "");
		return false;
	}

	// Create and fill GL shader program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	// Clean garbage
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// Check if linking succeded
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);

		LOGE_E("Error linking shader program: \n", infoLog);

		return false;
	}


	ID = program;
	UpdateUniformsLocations();
	return true;
}

void Shader::Use()
{
	if (!loaded)
		return;

	glUseProgram(ID);

	Camera* rendering = Camera::__GetRenderingCamera();
	if (rendering == nullptr)
	{
		LOGW_E("Rendering camera is nullptr");
		return;
	}

	glm::mat4 view = rendering->GetViewMatrix();
	SetMat4(viewLocation, glm::value_ptr(view));

	glm::mat4 projection = rendering->GetProjectionMatrix();
	SetMat4(projectionLocation, glm::value_ptr(projection));
}

void Shader::SetBool(GLint location, bool value)
{
	glUniform1i(location, value);
}

void Shader::SetInt(GLint location, int value)
{
	glUniform1i(location, value);
}

void Shader::SetFloat(GLint location, float value)
{
	glUniform1f(location, value);
}

void Shader::SetVec2(GLint location, float v1, float v2)
{
	glUniform2f(location, v1, v2);
}

void Shader::SetVec3(GLint location, float v1, float v2, float v3)
{
	glUniform3f(location, v1, v2, v3);
}

void Shader::SetVec4(GLint location, float v1, float v2, float v3, float v4)
{
	glUniform4f(location, v1, v2, v3, v4);
}

void Shader::SetMat4(GLint location, float* value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::SetBool(const string& param, bool value)
{
	SetBool(GetUniformLocation(param), value);
}

void Shader::SetInt(const string& param, int value)
{
	SetInt(GetUniformLocation(param), value);
}

void Shader::SetFloat(const string& param, float value)
{
	SetFloat(GetUniformLocation(param), value);
}

void Shader::SetVec2(const string& param, float v1, float v2)
{
	SetVec2(GetUniformLocation(param), v1, v2);
}

void Shader::SetVec3(const string& param, float v1, float v2, float v3)
{
	SetVec3(GetUniformLocation(param), v1, v2, v3);
}

void Shader::SetVec4(const string& param, float v1, float v2, float v3, float v4)
{
	SetVec4(GetUniformLocation(param), v1, v2, v3, v4);
}

void Shader::SetMat4(const string& param, float* value)
{
	SetMat4(GetUniformLocation(param), value);
}

/*void Shader::SetVertexBool(GLint location, bool value)
{
	glVertexAttribI1i(location, value);
}

void Shader::SetVertexInt(GLint location, int value)
{
	glVertexAttribI1i(location, value);
}

void Shader::SetVertexFloat(GLint location, float value)
{
	glVertexAttrib1f(location, value);
}

void Shader::SetVertexVec2(GLint location, float v1, float v2)
{
	glVertexAttrib2f(location, v1, v2);
}

void Shader::SetVertexVec3(GLint location, float v1, float v2, float v3)
{
	glVertexAttrib3f(location, v1, v2, v3);
}

void Shader::SetVertexVec4(GLint location, float v1, float v2, float v3, float v4)
{
	glVertexAttrib4f(location, v1, v2, v3, v4);
}

void Shader::SetVertexBool(const string& param, bool value)
{
	SetVertexBool(GetUniformLocation(param), value);
}

void Shader::SetVertexInt(const string& param, int value)
{
	SetVertexInt(GetUniformLocation(param), value);
}

void Shader::SetVertexFloat(const string& param, float value)
{
	SetVertexFloat(GetUniformLocation(param), value);
}

void Shader::SetVertexVec2(const string& param, float v1, float v2)
{
	SetVertexVec2(GetUniformLocation(param), v1, v2);
}

void Shader::SetVertexVec3(const string& param, float v1, float v2, float v3)
{
	SetVertexVec3(GetUniformLocation(param), v1, v2, v3);
}

void Shader::SetVertexVec4(const string& param, float v1, float v2, float v3, float v4)
{
	SetVertexVec4(GetUniformLocation(param), v1, v2, v3, v4);
}*/

GLint Shader::GetUniformLocation(string param)
{
	GLint v = glGetUniformLocation(ID, param.c_str());
	if (v < 0)
	{
		LOGE_E("Error getting uniform location for param: \"", param, "\"");
		//throw exception(("Error getting uniform location for param: " + param).c_str());
	}

	return v;
}

//Shader* Shader::defaultShader = nullptr;
//Asset<Shader> Shader::defaultShader;
shared_ptr<Shader> Shader::defaultShader;//(new Shader("VertexShader.vert", "FragmentShader.frag"));
shared_ptr<Shader> Shader::GetDefaultShader()
{
	if (defaultShader.get() == nullptr)
	{
		defaultShader = AssetsLoader::LoadShader("VertexShader.vert", "FragmentShader.frag");
	}
	return defaultShader;
}
