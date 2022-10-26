#pragma once

#include <glm/glm.hpp>
#include <limits>
#include <string>
#include "Util.h"

template<class Type>
class Vector3Base
{
public:
	Type x, y, z;

	float Magnitude() { return sqrt(this->SqrMagnitude()); }
	float SqrMagnitude() { return x * x + y * y + z * z; }

	Vector3Base<Type> Normalized()
	{
		glm::vec3 res = glm::normalize(glm::vec3(x, y, z));
		return Vector3Base<Type>(res.x, res.y, res.z);
	}
	void Normalize();

	Vector3Base<Type> Cross(Vector3Base b);
	float Dot(Vector3Base b);

	glm::vec3 GetGLVector();
	Vector3Base<Type>(Type x = 0, Type y = 0, Type z = 0) { this->x = x; this->y = y; this->z = z; }


	Vector3Base<Type> operator+(const Vector3Base& b) { return Vector3Base<Type>(x + b.x, y + b.y, z + b.z); }
	Vector3Base<Type> operator-(const Vector3Base& b) { return Vector3Base<Type>(x - b.x, y - b.y, z - b.z); }
	Vector3Base<Type>& operator+=(const Vector3Base& b) { x += b.x; y += b.y; z += b.z; return *this; }
	Vector3Base<Type> operator/(const Vector3Base& b) { return Vector3Base<Type>(x / b.x, y / b.y, z / b.z); }
	Vector3Base<Type> operator*(const Vector3Base& b) { return Vector3Base<Type>(x * b.x, y * b.y, z * b.z); }

	friend Vector3Base<Type> operator*(const Vector3Base<Type> a, float b) { return Vector3Base<Type>(a.x * b, a.y * b, a.z * b); }
	friend Vector3Base<Type> operator*(float b, const Vector3Base<Type> a) { return Vector3Base<Type>(b * a.x, b * a.y, b * a.z); }

	template<class Type2>
	Vector3Base<Type>& operator=(const Vector3Base<Type2>& b) { x = b.x; y = b.y; z = b.z; return *this; }
	
	Vector3Base<Type>& operator=(const glm::vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }

	std::string ToString() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	static Vector3Base<Type> Random() {
		return Random(0, std::numeric_limits<Type>::max() /* max value of this type */);
	}

	static Vector3Base<Type> Random(Type max) {
		return Random(0, max);
	}

	static Vector3Base<Type> Random(Type min, Type max) {
		return Vector3Base<Type>(GetRandomLong(min, max), GetRandomLong(min, max), GetRandomLong(min, max));
	}
};


using Vector3 = Vector3Base<float>;
using Vector3i = Vector3Base<int>;

template<class Type>
class Vector2Base
{
public:
	Type x, y;

	float Magnitude() { return sqrt(this->SqrMagnitude()); }
	float SqrMagnitude() { return x * x + y * y; }

	Vector2Base<Type> Normalized()
	{
		glm::vec2 v = glm::normalize(glm::vec2(x, y));
		return Vector2Base<Type>(v.x, v.y);
	}

	void Normalize();

	Vector2Base<Type> Cross(Vector2Base b);
	float Dot(Vector2Base b);

	glm::vec2 GetGLVector();
	Vector2Base<Type>(Type x = 0, Type y = 0) { this->x = x; this->y = y;}

	friend Vector2Base<Type> operator*(float a, const Vector2Base<Type>& b) { return Vector2Base<Type>(a * b.x, a * b.y); }
	friend Vector2Base<Type> operator*(const Vector2Base<Type>& b, float a) { return Vector2Base<Type>(b.x * a, b.y * a); }

	friend Vector2Base<Type> operator/(const Vector2Base<Type>& a, float b) { return Vector2Base<Type>(a.x / b, a.y / b); }
	friend Vector2Base<Type> operator/(float b, const Vector2Base<Type>& a) { return Vector2Base<Type>(b / a.x, b / a.y); }

	Vector2Base<Type> operator+(float b) { return Vector2Base<Type>(x + b, y + b); }
	Vector2Base<Type> operator-(float b) { return Vector2Base<Type>(x - b, y - b); }
	Vector2Base<Type> operator+(const Vector2Base& b) { return Vector2Base<Type>(x + b.x, y + b.y); }
	Vector2Base<Type> operator-(const Vector2Base& b) { return Vector2Base<Type>(x - b.x, y - b.y); }
	Vector2Base<Type> operator*(const Vector2Base& b) { return Vector2Base<Type>(x * b.x, y * b.y); }
	Vector2Base<Type> operator/(const Vector2Base& b) { return Vector2Base<Type>(x / b.x, y / b.y); }

	void operator+=(const Vector2Base& b) { x += b.x; y += b.y; }
	bool operator==(const Vector2Base& b) { return x == b.x && y == b.y; }
	bool operator!=(const Vector2Base& b) { return x != b.x || y != b.y; }

	operator Vector3Base<Type>() { return Vector3Base<Type>(x, y, 0); }
	operator Vector2Base<float>();

	template<class Type2>
	Vector2Base<Type>& operator=(const Vector2Base<Type2>& b) { x = b.x; y = b.y; return *this; }

	static Vector2Base<Type> Random()
	{
		return Random(0, std::numeric_limits<Type>::max());
	}

	static Vector2Base<Type> Random(Type max)
	{
		return Random(0, max);
	}

	static Vector2Base<Type> Random(Type min, Type max)
	{
		//srand(time(NULL));
		return Vector2Base<Type>((long)(rand() * 1000.0 / 50.0) % (long)(max - min) + min, (long)(rand() * 1512.0 / 156.0) % (long)(max-min) + min);
	}
};

using Vector2 = Vector2Base<float>;
using Vector2i = Vector2Base<int>;

template<class Type>
inline Vector3Base<Type> Vector3Base<Type>::Cross(Vector3Base b)
{
	glm::vec3 res = glm::cross(GetGLVector(), b.GetGLVector());
	return Vector3Base<Type>(res.x, res.y, res.z);
}

template<class Type>
inline float Vector3Base<Type>::Dot(Vector3Base b)
{
	return glm::dot(GetGLVector(), b.GetGLVector());
}

template<class Type>
inline glm::vec3 Vector3Base<Type>::GetGLVector()
{
	return glm::vec3(x, y, z);
}
