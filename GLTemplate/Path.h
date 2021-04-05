#pragma once

#include <string>
#include <initializer_list>
#include <vector>
#include "Asset.h"
#include "Util.h"
using namespace std;

class Path
{
public:
	const static string ASSETS_PATH;
	const static string IMAGES_PATH;
	const static string SEPARATOR;

	static string Combine(initializer_list<string> paths);
};