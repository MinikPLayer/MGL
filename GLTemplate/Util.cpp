#include "Util.h"
#include "GameObject.h"
#include <thread>

#include "stb_image.h"


int GetProcessorCount()
{
	return thread::hardware_concurrency();
}

string ReadAllText(const char* filePath)
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
	vector<string> ret;
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

	return ret;
}

bool CompileShader(const char* path, GLuint type, GLuint& result)
{
	
	string source = "";

	// Try to load shader content from file
	try
	{
		source = ReadAllText(path);
	}
	catch (IOException e)	// Loading failed
	{
		cout << "Error reading shader from \"" << path << "\"" << endl;
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

		cout << "Error compiling vertex shader: \n" << infoLog << endl;
		return false;
	}

	result = shader;
	return true;
}

GLint LoadTexture(const char* path, bool flip, TextureFiltering minFilter, TextureFiltering magFilter)
{
	// Load texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (!data)
	{
		LOGE_E("Failed to load [", path, "]  texture!");
		stbi_image_free(data);
		return -1;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	if (minFilter != TextureFiltering::None)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)minFilter);

	if (magFilter != TextureFiltering::None)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture;
}

