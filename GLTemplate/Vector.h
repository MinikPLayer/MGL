#pragma once

#include <glm/glm.hpp>
#include <ctime>
#include <limits>

template<class Type>
class Vector3Base
{
public:
	Type x, y, z;

	float Magnitude();
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

	Vector3Base<Type> operator*(float b) { return Vector3Base<Type>(x * b, y * b, z * b); }
	Vector3Base<Type> operator+(const Vector3Base& b) { return Vector3Base<Type>(x + b.x, y + b.y, z + b.z); }
	Vector3Base<Type> operator-(const Vector3Base& b) { return Vector3Base<Type>(x - b.x, y - b.y, z - b.z); }
	Vector3Base<Type>& operator+=(const Vector3Base& b) { x += b.x; y += b.y; z += b.z; return *this; }

	template<class Type2>
	Vector3Base<Type>& operator=(const Vector3Base<Type2>& b) { x = b.x; y = b.y; z = b.z; return *this; }

	static Vector3Base<Type> Random()
	{
		return Random(0, std::numeric_limits<Type>::max() /* max value of this type */);
	}

	static Vector3Base<Type> Random(Type max)
	{
		return Random(0, max);
	}

	static Vector3Base<Type> Random(Type min, Type max)
	{
		//srand(time(NULL));
		return Vector3Base<Type>((long)(rand() * 1000.0 / 50.0) % (long)(max - min) + min, (long)(rand() * 1512.0 / 156.0) % (long)(max - min) + min, (long)(rand() * 6312.0 / 362.0) % (long)(max - min) + min);
	}
};

#define Vector3 Vector3Base<float>
#define Vector3i Vector3Base<int>

/*class Vector3
{
public:
	float x, y, z;

	float Magnitude();
	Vector3 Normalized();
	void Normalize();

	Vector3 Cross(Vector3 b) { return glm::cross(GetGLVector(), b.GetGLVector()); }
	Vector3 Dot(Vector3 b) { return glm::dot(GetGLVector(), b.GetGLVector()); }

	glm::vec3 GetGLVector();

	Vector3(float x = 0, float y = 0, float z = 0);
	
	Vector3 operator*(float b) { return Vector3(x * b, y * b, z * b); }
	Vector3 operator+(const Vector3& b) { return Vector3(x + b.x, y + b.y, z + b.z); }
	Vector3 operator-(const Vector3& b) { return Vector3(x - b.x, y - b.y, z - b.z); }
	void operator+=(const Vector3& b) { x += b.x; y += b.y; z += b.z; }

protected:
	Vector3(const glm::vec3& v) { x = v.x; y = v.y; z = v.z; }
	void operator=(const glm::vec3& v) { x = v.x; y = v.y; z = v.z; }
};

class Vector3i
{
public:
	int x, y, z;

	float Magnitude();

	glm::vec3 GetGLVector();

	Vector3i(int x = 0, int y = 0, int z = 0);

	Vector3i operator+(const Vector3i& b) { return Vector3i(x + b.x, y + b.y, z + b.z); }
	Vector3i operator-(const Vector3i& b) { return Vector3i(x - b.x, y - b.y, z - b.z); }
};*/

template<class Type>
class Vector2Base
{
public:
	Type x, y;

	float Magnitude();
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

	Vector2Base<Type> operator*(float b) { return Vector2Base<Type>(x * b, y * b); }
	Vector2Base<Type> operator+(const Vector2Base& b) { return Vector2Base<Type>(x + b.x, y + b.y); }
	Vector2Base<Type> operator-(const Vector2Base& b) { return Vector2Base<Type>(x - b.x, y - b.y); }
	void operator+=(const Vector2Base& b) { x += b.x; y += b.y; }
	bool operator==(const Vector2Base& b) { return x == b.x && y == b.y; }
	bool operator!=(const Vector2Base& b) { return x != b.x || y != b.y; }

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

#define Vector2 Vector2Base<float>
#define Vector2i Vector2Base<int>

/*class Vector2
{
public:
	float x, y;

	float Magnitude();
	Vector2 Normalized();
	void Normalize();

	Vector2(float x = 0, float y = 0);
};

class Vector2i
{

};*/

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
