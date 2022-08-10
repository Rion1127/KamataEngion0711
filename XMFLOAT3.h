#pragma once
class XMFLOAT3
{
public:
	float x;
	float y;
	float z;

	

	XMFLOAT3 operator+() const;
	XMFLOAT3 operator-() const;

	XMFLOAT3& operator+=(const XMFLOAT3& v);
	XMFLOAT3& operator-=(const XMFLOAT3& v);
	XMFLOAT3& operator*=(const XMFLOAT3& v);
	XMFLOAT3& operator/=(const XMFLOAT3& v);
	XMFLOAT3& operator*=(float s);
	XMFLOAT3& operator/=(float s);

};

