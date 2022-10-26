#include "Vector.h"
#include <glm/glm.hpp>

template<>
void Vector3::Normalize()
{
    glm::vec3 v = glm::normalize(glm::vec3(x, y, z));
    x = v.x;
    y = v.y;
    z = v.z;
}

template<>
float Vector3i::Magnitude()
{
    return sqrt(x * x + y * y + z * z);
}

template<class Type>
inline void Vector2Base<Type>::Normalize()
{
    glm::vec2 v = glm::normalize(glm::vec2(x, y));
    x = v.x;
    y = v.y;
}

template<>
Vector2Base<int>::operator Vector2Base<float>()
{   
    return Vector2Base<float>(x, y);
}

