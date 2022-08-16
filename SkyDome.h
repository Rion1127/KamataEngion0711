#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Matrix.h"
class SkyDome
{
public:
	~SkyDome();
	void Ini(Model* model);
	void Update();
	void Draw(ViewProjection viewProjection);

private:
	WorldTransform worldTransform;
	Matrix matrix;
	Model* model_ = nullptr;
};

