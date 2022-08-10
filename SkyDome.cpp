#include "ViewProjection.h"
#include "SkyDome.h"

SkyDome::~SkyDome()
{
	
}

void SkyDome::Ini(Model* model)
{
	worldTransform.Initialize();
	worldTransform.translation_ = { 0,0,0 };
	model_ = model;
	
}

void SkyDome::Update()
{
}

void SkyDome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform, viewProjection);
}
