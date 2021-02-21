#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include "GameObject.h"

using namespace std;


extern vector<GameObject*> objects;

template<class Type>
Type* Instantiate()
{
	Type* t = new Type();
	objects.push_back(t);
	t->Start();
	return t;
}

template<class Type>
Type* Instantiate(Type* prefab)
{
	//Type* t = new Type();
	//prefab->SetPrefab(t);
	//t->CopyValues(prefab);
	objects.push_back(prefab);

	prefab->Start();
	return prefab;
}

class IOException : exception
{
public:
	IOException(const char* error)
		:exception(error) {}
};

/// <summary>
/// Read all text from specified file
/// </summary>
/// <param name="filePath">Path to a file to be read</param>
/// <returns>Contents of the specified file</returns>
string ReadAllText(const char* filePath);

/// <summary>
/// Compiles a shader from specified file
/// </summary>
/// <param name="path">Path to the shader file</param>
/// <param name="type">Type of shader ( GL_VERTEX_SHADER or GL_FRAGMENT_SHADER for example )</param>
/// <returns>True if loaded and compiled successfully, false otherwise</returns>
bool CompileShader(const char* path, GLuint type, GLuint& result);