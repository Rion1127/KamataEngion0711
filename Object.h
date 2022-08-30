#pragma once
#include "WorldTransform.h"
#include "MathUtility.h"
#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "ViewProjection.h"
class BlueOrangeObject
{
public:
	void Ini(Model* model,WorldTransform worldTransform);
	void Update();
	void Draw( ViewProjection viewProjection);
	

	WorldTransform worldTransform_;
private:
	Matrix matrix;
	Model* model_;
	
};

class BoxObject
{
public:
	void Ini(Model* model, WorldTransform worldTransform);
	void Update();
	void Draw(ViewProjection viewProjection, uint32_t textureHandle);
	

	WorldTransform worldTransform_;
private:
	Matrix matrix;
	Model* model_;
	
};

