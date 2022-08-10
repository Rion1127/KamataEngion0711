#pragma once
#include "Model.h"
#include "WorldTransform.h"
class SkyDome
{
public:
	~SkyDome();
	void Ini(Model* model);
	void Update();
	void Draw(ViewProjection viewProjection);

private:
	WorldTransform worldTransform;
	Model* model_ = nullptr;
};

