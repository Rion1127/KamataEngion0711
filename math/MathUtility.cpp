#include "MathUtility.h"

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
    //�⊮���ׂ��_�̐�
    //size_t n = point.size() - 2;

    //if (startIndex > n)return point[n];//Pn�̒l��Ԃ�
    //if (startIndex > 1)return point[1];//P1�̒l��Ԃ�

    ////p0~p3�̐���_���擾����@��p1~p2��⊮����
    //Vector3 p0 = point[startIndex - 1];
    //Vector3 p1 = point[startIndex];
    //Vector3 p2 = point[startIndex + 1];
    //Vector3 p3 = point[startIndex + 2];

    ////Catmull-Rom�̎��ɂ��⊮
    //Vector3 position = 0.5f * {(2 * p1) + ((-p0 + p2) * t) +
    //    ((2 * p0) - (5 * p1) + (4 * p2) - p3) * (t * t) +
    //    (-p0 + (3 * p1) - (3 * p2) + p3) * (t * t * t)};

    Vector3 position = { 0,0,0 };

    return position;
}
