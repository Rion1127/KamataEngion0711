#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
class RailCamera
{
public:
	void Ini(Vector3 pos,Vector3 rot);
	void Update();

	ViewProjection GetViewProjection() { return viewProjection; }

private:
	WorldTransform worldTransform;
	ViewProjection viewProjection;

	DebugText* debugText_ = nullptr;
};

