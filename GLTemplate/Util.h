#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <functional>
#include "Debug.h"
#include "Vector.h"

using namespace std;


class Sky;

class IOException : exception
{
public:
	IOException(const char* error)
		:exception(error) {}
};


template<class T1, class T2>
struct Tuple2
{
public:
	T1 item1;
	T2 item2;

	Tuple2(T1 i1, T2 i2) {
		item1 = i1;
		item2 = i2;
	}
};


void SplitToThreads1D(int start, int end, std::function<void(int)> func, int threadCount = 0);


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

class StrUtils
{
public:
	/// <summary>
	/// Replaces any replaceStr occurance in src string, changes nothing if match is not found
	/// </summary>
	/// <param name="src">Source string</param>
	/// <param name="match">Match string</param>
	/// <param name="replaceStr">String to paste into</param>
	/// <returns>String after replacing</returns>
	static string Replace(string src, string match, string replaceStr);

	/// <summary>
	/// Checks if src string starts with match string
	/// </summary>
	/// <param name="src">Source string</param>
	/// <param name="match">Match to find</param>
	/// <returns>True if src starts with match</returns>
	static bool StartsWith(string src, string match);

	/// <summary>
	/// Replaces src match with replaceStr if src starts with match
	/// </summary>
	/// <param name="src">Source string</param>
	/// <param name="match">Match to find</param>
	/// <param name="replaceStr">String to paste instead of match</param>
	/// <returns>Modified string</returns>
	static string ReplaceStart(string src, string match, string replaceStr);

	/// <summary>
	/// Splits string at character c
	/// </summary>
	/// <param name="src">Source string</param>
	/// <param name="c">Split character</param>
	/// <returns>Splitted strings</returns>
	static vector<string> Split(string src, char c, int maxSplitCount = -1);
};



/// <summary>
/// Compiles a shader from specified file
/// </summary>
/// <param name="path">Path to the shader file</param>
/// <param name="type">Type of shader ( GL_VERTEX_SHADER or GL_FRAGMENT_SHADER for example )</param>
/// <returns>True if loaded and compiled successfully, false otherwise</returns>
bool CompileShader(string path, GLuint type, GLuint& result);

double deg2rad(double deg);
double rad2deg(double rad);

enum class TextureFiltering
{
	None,
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR
};

struct TextureInfo
{
public:
	GLint ID;
	int channels;

	TextureInfo(GLint ID, int channels);
};

/// <summary>
/// Loads texture from file
/// </summary>
/// <param name="path">Path to image file</param>
/// <param name="flip">True if you want to flip the image vertically (needed in some formats like BMP or PNG)</param>
/// <param name="minFilter">Minimization filter, defaults to Nearest</param>
/// <param name="magFilter">Maginifaction filter, defaults to Nearest</param>
/// <returns>GL handle to texture</returns>
TextureInfo LoadTexture(const char* path, bool flip = true, TextureFiltering minFilter = TextureFiltering::Nearest, TextureFiltering magFilter = TextureFiltering::Nearest);

extern Sky* sky;

//#include "GameObject.h"
