
#include "MathUtility.h"
#define PI 3.141592

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	// float y = t;
	// return start * (1.0f - y) + end * y;
	return start * (1.0f - t) + end * t;
}

const Vector3 ease_in(const Vector3& start, const Vector3& end, const float t)
{
	//y = t^2
	return start * (1.0f - t) + end * (t * t);
}

const Vector3 ease_out(const Vector3& start, const Vector3& end, const float t)
{
	//y = t(2 - t)
	return start * (1.0f - t) + end * (t * (2 - t));
}

const Vector3 ease_in_out(const Vector3& start, const Vector3& end, const float t)
{
	//y = t^2 (3 - 2t)
	return start * (1.0f - t) + end * (t * t * (3 - (2 * t)));
}

const Vector3 SplinePosition(const std::vector<Vector3>& point, size_t startIndex, const float t)
{
	////補完すべき点の数
	size_t n = point.size() - 2;

	if (startIndex > n)return point[n];//Pnの値を返す
	if (startIndex < 1)return point[1];//P1の値を返す

	//p0~p3の制御点を取得する　※p1~p2を補完する
	Vector3 p0 = point[startIndex];
	Vector3 p1 = point[startIndex + 1];
	Vector3 p2;
	if (startIndex > 0)
	{
		p2 = 0.5f * (point[startIndex + 1] - point[startIndex - 1]);
	}
	else
	{
		p2 = point[startIndex + 1] - point[startIndex];
	}
	Vector3 p3;
	if (startIndex < n)
	{
		p3 = 0.5f * (point[startIndex + 2] - point[startIndex]);
	}
	else
	{
		p3 = point[startIndex + 1] - point[startIndex];
	}

	Vector3 position = GetPoint(p0, p1, p2, p3, t);

	return position;
}

// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
Vector3 GetPoint(Vector3 p0, Vector3 p1, Vector3 v0, Vector3 v1, float t)
{
	Vector3 c0 = 2.0f * p0 + -2.0f * p1 + v0 + v1;
	Vector3 c1 = -3.0f * p0 + 3.0f * p1 + -2.0f * v0 - v1;
	Vector3 c2 = v0;
	Vector3 c3 = p0;

	float t2 = t * t;
	float t3 = t2 * t;
	return c0 * t3 + c1 * t2 + c2 * t + c3;
}

#pragma region 変換
//ラジアンから角度
float ConvertRadianToAngle(float radian) {
	float angle = 180 / PI * radian;
	return angle;
}

//角度からラジアン
float ConvertAngleToRadian(float angle) {
	float radian = PI / 180 * angle;
	return radian;
}

Vector3 ConvertWorldToScreen(Vector3 p, Matrix4 m)
{
	float w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + m.m[3][0]) / w,
		(p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + m.m[3][1]) / w,
		(p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

Vector3 Vector3toTransform(Vector3 v, Matrix4 m)
{
	Vector3 vec3{};

	vec3.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
	vec3.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
	vec3.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);

	return vec3;
}

Vector3 Vector3toTransformW(Vector3 v, Matrix4 m)
{
	Vector3 vec3{};
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	vec3.x = ((v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0])) / w;
	vec3.y = ((v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1])) / w;
	vec3.z = ((v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2])) / w;

	return vec3;
}

Vector4 A(Vector4 v, Matrix4 m)
{
	Vector4 result = {
		m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
		m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
	};

	return result;
}

Vector4 B(Vector4 v, Matrix4 m)
{
	Vector4 result = {
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w,
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w,
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w,
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w,
	};

	return result;
}

Vector4 W(Vector4 v)
{
	Vector4 result = {
		v.x / v.w,
		v.y / v.w,
		v.z / v.w,
		v.w / v.w,
	};
	return result;
}

#pragma endregion