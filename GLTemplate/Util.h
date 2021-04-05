#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include "Debug.h"
#include "Vector.h"

using namespace std;




class IOException : exception
{
public:
	IOException(const char* error)
		:exception(error) {}
};

int GetProcessorCount();

/// <summary>
/// Reads text from specified file
/// </summary>
/// <param name="filePath">Path to a file to be read</param>
/// <returns>Contents of the specified file</returns>
string ReadAllText(string filePath);

/// <summary>
/// Reads lines from specified file
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
vector<string> ReadAllLines(const char* filePath);

/// <summary>
/// Compiles a shader from specified file
/// </summary>
/// <param name="path">Path to the shader file</param>
/// <param name="type">Type of shader ( GL_VERTEX_SHADER or GL_FRAGMENT_SHADER for example )</param>
/// <returns>True if loaded and compiled successfully, false otherwise</returns>
bool CompileShader(string path, GLuint type, GLuint& result);

enum class TextureFiltering
{
	None,
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR
};

/// <summary>
/// Loads texture from file
/// </summary>
/// <param name="path">Path to image file</param>
/// <param name="flip">True if you want to flip the image vertically (needed in some formats like BMP or PNG)</param>
/// <param name="minFilter">Minimization filter, defaults to Nearest</param>
/// <param name="magFilter">Maginifaction filter, defaults to Nearest</param>
/// <returns>GL handle to texture</returns>
GLint LoadTexture(const char* path, bool flip = true, TextureFiltering minFilter = TextureFiltering::Nearest, TextureFiltering magFilter = TextureFiltering::Nearest);

//#include "GameObject.h"
