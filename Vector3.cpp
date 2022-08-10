#include "Vector3.h"
#include <cmath>    // sqrt

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
// �x�N�g���̃m����(����)�����߂�
float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}
// �x�N�g���𐳋K��
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}
// ���ς����߂�
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}
// �O�ς����߂�
Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}
// �P�����Z�q�I�[�o�[���[�h
//�P�����Z�q+���g����悤�ɂ���
Vector3 Vector3::operator+() const
{
	return *this;
}
//	�V	�@-���g����悤�ɂ���
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(Vector3 v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

// ������Z�q�I�[�o�[���[�h
//+=���g�p�ł���悤�ɂ���
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
//-=���g�p�ł���悤�ɂ���
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
//�x�N�g����S�{�ɂ���
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
//�x�N�g����1/S�{�ɂ���
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3& Vector3::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector3& Vector3::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}
