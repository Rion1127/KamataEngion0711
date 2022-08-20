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
    return start * (1.0f - t) + end * (t * t * (3- (2 * t)));
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

    Vector3 position = GetPoint(p0,p1,p2,p3,t);

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

#pragma endregion