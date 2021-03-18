#include "Vector.h"

#include <glm/glm.hpp>

float Vector3::Magnitude()
{
    return glm::vec3(x, y, z).length();
}

Vector3 Vector3::Normalized()
{
    glm::vec3 res = glm::normalize(glm::vec3(x, y, z));
    return Vector3(res.x, res.y, res.z);
}

void Vector3::Normalize()
{
    glm::vec3 v = glm::normalize(glm::vec3(x, y, z));
    x = v.x;
    y = v.y;
    z = v.z;
}

float Vector3i::Magnitude()
{
    return sqrt(x * x + y * y + z * z);
}

/*Vector3i::Vector3i(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}*/

template<class Type>
inline float Vector2Base<Type>::Magnitude()
{
    return sqrt(x * x + y * y);
}

template<class Type>
inline Vector2Base<Type> Vector2Base<Type>::Normalized()
{
    glm::vec2 v = glm::normalize(glm::vec2(x, y));
    return Vector2Base<Type>(v.x, v.y);
}

template<class Type>
inline void Vector2Base<Type>::Normalize()
{
    glm::vec2 v = glm::normalize(glm::vec2(x, y));
    x = v.x;
    y = v.y;
}
