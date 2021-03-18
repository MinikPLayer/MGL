#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>

using namespace std;

class Shader
{
	bool loaded = false;
	string error = "";

	static Shader* defaultShader;

public:
	// Locations of uniforms
	GLint modelLocation = 0;
	GLint viewLocation = 0;
	GLint projectionLocation = 0;
	//GLint shaderModelPosition = 0;

	void UpdateUniformsLocations();

public:
	const static string TYPE_VERTEX;
	const static string TYPE_FRAGMENT;
	
	class ShaderLoadException : exception
	{
	public:
		ShaderLoadException(string type, string message)
			:exception(("Error loading " + type + " shader, error: " + message).c_str()) {}
	};

	GLuint ID = 0;

	Shader(const char* vertexPath, const char* fragmentPath);

	/// <summary>
	/// Load vertex and fragment shader from file, compiles it and combines them into a program
	/// </summary>
	/// <param name="vertexPath">Path to vertex shader</param>
	/// <param name="fragmentPath">Path to fragment shader</param>
	/// <returns>True if loaded and compiled successfully, false otherwise</returns>
	bool LoadShaders(const char* vertexPath, const char* fragmentPath);

	void Use();

	// Uniform parameters wrappers
	void SetBool(GLint location, bool value);
	void SetInt(GLint location, int value);
	void SetFloat(GLint location, float value);
	void SetVec2(GLint location, float v1, float v2);
	void SetVec3(GLint location, float v1, float v2, float v3);
	void SetVec4(GLint location, float v1, float v2, float v3, float v4);
	void SetMat4(GLint location, float* value);

	void SetBool(const string& param, bool value);
	void SetInt(const string& param, int value);
	void SetFloat(const string& param, float value);
	void SetVec2(const string& param, float v1, float v2);
	void SetVec3(const string& param, float v1, float v2, float v3);
	void SetVec4(const string& param, float v1, float v2, float v3, float v4);
	void SetMat4(const string& param, float* value);

	/*// Vertex parameters wrappers
	void SetVertexBool(GLint location, bool value);
	void SetVertexInt(GLint location, int value);
	void SetVertexFloat(GLint location, float value);
	void SetVertexVec2(GLint location, float v1, float v2);
	void SetVertexVec3(GLint location, float v1, float v2, float v3);
	void SetVertexVec4(GLint location, float v1, float v2, float v3, float v4);

	void SetVertexBool(const string& param, bool value);
	void SetVertexInt(const string& param, int value);
	void SetVertexFloat(const string& param, float value);
	void SetVertexVec2(const string& param, float v1, float v2);
	void SetVertexVec3(const string& param, float v1, float v2, float v3);
	void SetVertexVec4(const string& param, float v1, float v2, float v3, float v4);*/

	GLint GetUniformLocation(string param);

	static Shader* GetDefaultShader();
};