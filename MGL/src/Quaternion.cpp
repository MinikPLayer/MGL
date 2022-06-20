#include "Quaternion.h"
#include "Util.h"

Quaternion Quaternion::FromEuler(Vector3 euler)
{
    Quaternion q;
    q.quaternion = glm::quat(euler.GetGLVector());
    return q;
}

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
    Quaternion q;
    q.quaternion = glm::quat(glm::vec3(x, y, z));
    return q;
}

Vector3 Quaternion::ToEuler()
{
    glm::vec3 v = glm::eulerAngles(quaternion);
    return Vector3(v.x, v.y, v.z);
}

glm::quat Quaternion::GetGlVector()
{
    return quaternion;
}

Quaternion::Quaternion(float x, float y, float z)
{
    this->quaternion = glm::quat(glm::vec3(x, y, z));
}

Quaternion::Quaternion(Vector3 rot)
{
    rot = Vector3(deg2rad(rot.x), deg2rad(rot.y), deg2rad(rot.z));
    this->quaternion = glm::quat(rot.GetGLVector());
}

Quaternion Quaternion::operator+(Quaternion q)
{
    Quaternion ret;
    ret.quaternion = quaternion * q.quaternion;
    return ret;
}

Quaternion Quaternion::operator-(Quaternion q)
{
    Quaternion ret;
    ret.quaternion = quaternion * glm::inverse(q.quaternion);
    return ret;
}

Quaternion Quaternion::operator+(Vector3 euler)
{
    Quaternion ret;
    ret.quaternion = quaternion * glm::quat(euler.GetGLVector());
    return ret;
}

Quaternion Quaternion::operator-(Vector3 euler)
{
    Quaternion ret;
    ret.quaternion = quaternion * glm::quat((euler * -1).GetGLVector());
    return ret;
}
