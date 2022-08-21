#include "WorldTransform.h"

void WorldTransform::AddScale(float x, float y, float z)
{
	scale_.x += x;
	scale_.y += y;
	scale_.z += z;
}

void WorldTransform::AddRotation(float x, float y, float z)
{
	rotation_.x += x;
	rotation_.y += y;
	rotation_.z += z;
}

void WorldTransform::AddPosition(float x, float y, float z)
{
	translation_.x += x;
	translation_.y += y;
	translation_.z += z;
}

void WorldTransform::AddScale(Vector3 scale)
{
	scale_.x += scale.x;
	scale_.y += scale.y;
	scale_.z += scale.z;
}

void WorldTransform::AddRotation(Vector3 rot)
{
	rotation_.x += rot.x;
	rotation_.y += rot.y;
	rotation_.z += rot.z;
}

void WorldTransform::AddPosition(Vector3 pos)
{
	translation_.x += pos.x;
	translation_.y += pos.y;
	translation_.z += pos.z;
}

Vector3 WorldTransform::GetWorldPosition()
{
	Vector3 worldPos =
	{
		matWorld_.m[3][0],
		matWorld_.m[3][1],
		matWorld_.m[3][2]
	};
	return worldPos;
}
