#include "Path.h"
#include <stdarg.h>

const string Path::ASSETS_PATH = "Assets";
#ifdef __linux__
const string Path::SEPARATOR = "/"; // Linux
#elif _WIN32
const string Path::SEPARATOR = "\\"; // Windows
#else
// Other
#error Platform not supported
#endif

const string Path::IMAGES_PATH = Path::Combine({ Path::ASSETS_PATH, "Images" });


#define __COMBINE_PATH__(...) 

string Path::Combine(initializer_list<string> paths)
{
	string path = "";
	for (auto p : paths)
	{
		path += p + Path::SEPARATOR;
	}

	// Remove separator at the end
	if (path.length() != 0)
		path.erase(path.begin() + path.length() - 1);

	return path;
}
