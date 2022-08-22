#pragma once
#include "WorldTransform.h"


struct MatResult {
	Matrix4 Scale;
	Matrix4 Rot;
	Matrix4 Trans;
};

class Matrix
{
public:
	Matrix();

	void UnitMatrix(Matrix4& m);
	//初期設定
	void ScaleChange(WorldTransform& worldTransform, float m1, float m2, float m3, float m4);

	void RotaChange(WorldTransform& worldTransform, float m1, float m2, float m3);

	void ChangeTranslation(WorldTransform& worldTransform, float tx, float ty, float tz);

	//初期設定
	void ScaleChange(WorldTransform& worldTransform, WorldTransform scale);

	void RotaChange(WorldTransform& worldTransform, WorldTransform rot);

	void ChangeTranslation(WorldTransform& worldTransform, WorldTransform trans);

	//連続処理
	void ScaleChange(WorldTransform& worldTransform);

	void RotaChange(WorldTransform& worldTransform);

	void ChangeTranslation(WorldTransform& worldTransform);

	void UpdateMatrix(WorldTransform& worldTransform);

private:
	MatResult matResult;
};

