#include "Object.h"

void BlueOrangeObject::Ini(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.scale_ = worldTransform.scale_;
	matrix.UpdateMatrix(worldTransform_);
}

void BlueOrangeObject::Update()
{

}

void BlueOrangeObject::Draw(ViewProjection viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void BoxObject::Ini(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.scale_ = worldTransform.scale_;
	matrix.UpdateMatrix(worldTransform_);
}

void BoxObject::Update()
{
}

void BoxObject::Draw(ViewProjection viewProjection, uint32_t textureHandle)
{
	
	model_->Draw(worldTransform_, viewProjection, textureHandle);
	
}
