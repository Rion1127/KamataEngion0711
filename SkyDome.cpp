#include "ViewProjection.h"
#include "SkyDome.h"

SkyDome::~SkyDome()
{
	
}

void SkyDome::Ini(Model* model)
{
	worldTransform.Initialize();
	worldTransform.translation_ = { 0,0,0 };
	worldTransform.scale_ = { 5,5,5 };
	worldTransform.rotation_ = { 0,0,0 };
	matrix.UpdateMatrix(worldTransform);
	model_ = model;

	rot = { 0,0,0 };
	
}

void SkyDome::Update()
{
	Vector3 rotSpeed = { 0.00016f,0.00016f,0.000008f };

	worldTransform.AddRotation(rotSpeed);
	matrix.UpdateMatrix(worldTransform);
}

void SkyDome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform, viewProjection);
}
