#pragma once
#include "Matrix.h"
#include <cassert>

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
class EnemyBullet
{
public:
	EnemyBullet();
	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);
	WorldTransform GetWorldTransform() { return worldTransform_; }

	bool IsDead() const { return isDead_; }

	void OnCollisioin();

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//寿命
	static const int32_t lifeTime = 60 * 2;
	//デスタイマー
	int32_t deathTimer_ = lifeTime;
	//デスフラグ
	bool isDead_ = false;

	//速度
	Vector3 velocity_;
};
