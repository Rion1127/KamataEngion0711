#include "XMFLOAT3.h"
#include "Vector3.h"

XMFLOAT3 XMFLOAT3::operator+() const
{
    return *this;
}

XMFLOAT3 XMFLOAT3::operator-() const
{
    return *this;
}

XMFLOAT3& XMFLOAT3::operator+=(const XMFLOAT3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

XMFLOAT3& XMFLOAT3::operator-=(const XMFLOAT3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

XMFLOAT3& XMFLOAT3::operator*=(const XMFLOAT3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

XMFLOAT3& XMFLOAT3::operator/=(const XMFLOAT3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

XMFLOAT3& XMFLOAT3::operator*=(float s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

XMFLOAT3& XMFLOAT3::operator/=(float s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}
