#include "Color.h"

Color::Color(int r, int g, int b, int a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color Color::FromRGB(int r, int b, int g, int a)
{
	return Color(r, g, b, a);
}

Vector3 Color::ToVector3()
{
	// Normalized
	float rn = r / 255.f;
	float gn = g / 255.f;
	float bn = b / 255.f;
	float an = a / 255.f;

	return Vector3(rn * an, gn * an, bn * an);
}

Color Color::White = Color(255,255,255);
Color Color::Black = Color(0, 0, 0);
Color Color::Transparent = Color(0, 0, 0, 0);
Color Color::Red = Color(255, 0, 0);
Color Color::Green = Color(0, 255, 0);
Color Color::Blue = Color(0, 0, 255);

Color Color::Random() {
    auto vec = Vector3i::Random(0, 255);
    return {vec.x, vec.y, vec.z};
}
