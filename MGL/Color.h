#pragma once

#include "Vector.h"

#define byte unsigned char

class Color
{
public:
	byte r, g, b, a;

	Color(int r = 0, int g = 0, int b = 0, int a = 255);

	static Color FromRGB(int r, int b, int g, int a = 255);
    static Color Random();

	Vector3 ToVector3();

	static Color White;
	static Color Black;
	static Color Transparent;
	static Color Red;
	static Color Green;
	static Color Blue;
};