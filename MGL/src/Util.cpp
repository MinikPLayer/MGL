#include "Util.h"
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SystemInfo.h"

#include "Sky.h"

Sky* sky;

void SplitToThreads1D(int start, int end, std::function<void(int)> func, int threadCount)
{
	if (end - start < 1) {
		return;
	}

	if (threadCount == 0) {
		threadCount = SystemInfo::GetThreadCount() - 1;
	}

	if (end - start < threadCount) {
		threadCount = end - start;
	}

	thread** threads = new thread * [threadCount];
	int* startOffsets = new int[threadCount];
	int* sizes = new int[threadCount];

	int left = end - start;
	int step = left / threadCount;
	for (int i = 0; i < threadCount; i++) {
		startOffsets[i] = start + step * i;
		sizes[i] = step;
		left -= step;
	}

	sizes[threadCount - 1] += left;
	left = 0;

	for (int i = 0; i < threadCount; i++) {
		threads[i] = new thread([&func, startOffsets, sizes, i]() {
			for (int j = 0; j < sizes[i]; j++) {
				func(startOffsets[i] + j);
			}
		});
	}

	for (int i = 0; i < threadCount; i++) {
		threads[i]->join();
		delete threads[i];
	}

	delete[] threads;
	delete[] startOffsets;
	delete[] sizes;
}

string ReadAllText(string filePath)
{
	string data = "";
	
	// Open file and check if it's ready to read
	ifstream file(filePath);
	if (!file.good())
	{
		throw IOException("Error reading file");
		return "";
	}

	// Check file length and assign memory to string
	file.seekg(0, std::ios::end);
	data.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	// Copy file content to string
	data.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return data;
}

vector<string> ReadAllLines(const char* filePath)
{
	string content = ReadAllText(filePath);
	/*vector<string> ret;
	int startIndex = 0;
	for (int i = 0; i < content.size(); i++)
	{
		if (content[i] == '\n')
		{
			ret.push_back(content.substr(startIndex, i - startIndex));
			startIndex = i + 1;
		}
	}

	if (startIndex < content.size())
		ret.push_back(content.substr(startIndex, content.size() - startIndex));

	return ret;*/

	return StrUtils::Split(content, '\n');
}

string StrUtils::Replace(string src, string match, string replaceStr)
{
	if (replaceStr.empty() || src.empty())
		return src;

	size_t start_pos = 0;
	while ((start_pos = src.find(match, start_pos)) != std::string::npos) {
		src.replace(start_pos, match.length(), replaceStr);
		start_pos += replaceStr.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}

	return src;
}

bool StrUtils::StartsWith(string src, string match)
{
	if (match.size() > src.size())
		return false;

	for (int i = 0; i < src.size() && i < match.size(); i++)
		if (src[i] != match[i])
			return false;

	return true;
}

string StrUtils::ReplaceStart(string src, string match, string replaceStr)
{
	if (!StartsWith(src, match))
		return src;

	src.erase(0, match.size());
	src.insert(0, replaceStr);

	return src;
}

vector<string> StrUtils::Split(string src, char c, int maxSplitCount)
{
	vector<string> ret;
	ret.push_back("");
	int ind = 0;
	for (int i = 0; i < src.size(); i++)
	{
		if (src[i] == c && (ret.size() < maxSplitCount || maxSplitCount == -1))
		{
			ret.push_back("");
			ind++;
			continue;
		}

		ret[ind] += src[i];
	}

	return ret;
}

bool CompileShader(string path, GLuint type, GLuint& result)
{
	
	string source = "";

	// Try to load shader content from file
	try
	{
		source = ReadAllText(path);
	}
	catch (IOException e)	// Loading failed
	{
		LOGE_E("Error reading shader from \"", path, "\"");
		return -1;
	}

	// Create and compile shader with specified type
	GLuint shader = glCreateShader(type);
	const GLchar* vSource = source.c_str();
	glShaderSource(shader, 1, &vSource, NULL);
	glCompileShader(shader);

	// Check for compilation status
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		LOGE_E("Error compiling vertex shader: \n", infoLog);
		return false;
	}

	result = shader;
	return true;
}

#define PI 3.14159265
double deg2rad(double deg)
{
	return deg * PI / 180.0;
}

double rad2deg(double rad)
{
	return rad * 180.0 / PI;
}

TextureInfo LoadTexture(const char* path, TextureFiltering minFilter, TextureFiltering magFilter, TextureWrapping wrapHor, TextureWrapping wrapVert, bool flip)
{
	// Load texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (!data)
	{
		LOGE_E("Failed to load [", path, "]  texture!");
		stbi_image_free(data);
		return TextureInfo(-1, -1);
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	if (minFilter != TextureFiltering::None)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)minFilter);

	if (magFilter != TextureFiltering::None)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)magFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrapHor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrapVert);

	if (nrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (nrChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else if (nrChannels == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	else
		LOGE_E("Bad texture channels count: ", nrChannels);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return TextureInfo(texture, nrChannels);
}

TextureFiltering defaultFilter = TextureFiltering::Linear;
TextureWrapping defaultWrapping = TextureWrapping::Repeat;

TextureInfo LoadTexture(const char* path, bool flip) {
	return LoadTexture(path, defaultFilter, defaultFilter, defaultWrapping, defaultWrapping, flip);
}

TextureInfo LoadTexture(const char* path, TextureFiltering minFilter, TextureFiltering magFilter, bool flip) {
	return LoadTexture(path, minFilter, magFilter, defaultWrapping, defaultWrapping, flip);
}

TextureInfo LoadTexture(const char* path, TextureFiltering filter, bool flip) {
	return LoadTexture(path, filter, filter, defaultWrapping, defaultWrapping, flip);
}

TextureInfo LoadTexture(const char* path, TextureWrapping wrapHor, TextureWrapping wrapVert, bool flip) {
	return LoadTexture(path, defaultFilter, defaultFilter, wrapHor, wrapVert, flip);
}

TextureInfo LoadTexture(const char* path, TextureWrapping wrapping, bool flip) {
	return LoadTexture(path, defaultFilter, defaultFilter, wrapping, wrapping, flip);
}

TextureInfo::TextureInfo(GLint ID, int channels)
{
	this->ID = ID;
	this->channels = channels;
}
