#include "EnemyBullet.h"

EnemyBullet::EnemyBullet()
{
}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3 rot, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("bullet.png");

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 0.2f, 0.1f, 3, 1);
	matrix.RotaChange(worldTransform_, rot.x, rot.y, rot.z);
	worldTransform_.translation_ = position;
	matrix.UpdateMatrix(worldTransform_);

	velocity_ = velocity;

}

void EnemyBullet::Update()
{
	//座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
	matrix.UpdateMatrix(worldTransform_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	
}

void EnemyBullet::OnCollisioin()
{
	isDead_ = true;
}
