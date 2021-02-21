#include "Util.h"

vector<GameObject*> objects;

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


