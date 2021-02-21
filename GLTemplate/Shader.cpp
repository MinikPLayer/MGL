#include "Shader.h"

#include "Util.h"

const string Shader::TYPE_FRAGMENT = "fragment";
const string Shader::TYPE_VERTEX = "vertex";

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	loaded = LoadShaders(vertexPath, fragmentPath);
}

bool Shader::LoadShaders(const char* vertexPath, const char* fragmentPath)
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

		cout << "Error linking shader program: \n" << infoLog << endl;

		return false;
	}


	ID = program;
	return true;
}

void Shader::Use()
{
	if (!loaded)
		return;

	glUseProgram(ID);
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
	return glGetUniformLocation(ID, param.c_str());
}

Shader* Shader::defaultShader = nullptr;
Shader* Shader::GetDefaultShader()
{
	if (defaultShader == nullptr)
	{
		defaultShader = new Shader("VertexShader.vert", "FragmentShader.frag");
	}

	return defaultShader;
}
