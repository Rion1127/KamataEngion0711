#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include <memory>
#include <list>
#include "Player.h"
#include "Enemy.h"

Enemy::Enemy()
{
	
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//テクスチャ読み込み
	textureHandle_ = textureHandle;
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, 3, 50);
	matrix.UpdateMatrix(worldTransform_);


	//Shot();

	phese_ApproachIni();
}

void Enemy::Update()
{
	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	matrix.UpdateMatrix(worldTransform_);
}

void Enemy::Shot()
{
	assert(player_);

	// 弾の速度
	const float kBulletSpped = 0.5f;

	// 差分ベクトルを格納する変数
	Vector3 diffVec;

	// 自キャラ、敵キャラのワールド座標を取得
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = worldTransform_.translation_;

	// 差分ベクトルを求める
	diffVec = enemyPos - playerPos;

	// 差分ベクトルの正規化
	diffVec.normalize();

	// ベクトルの長さを、速さに合わせる。
	diffVec *= kBulletSpped;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, diffVec);
	// 弾を登録する
	bullets_.push_back(std::move(newBullet));

	
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	//デバッグ表示
	debugText_->SetPos(50, 190);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"Phase:%d",
		phase_);
}

void Enemy::OnCollisioin()
{
}

void Enemy::phase_Approach()
{
	speed = { 0,0,-0.001f };
	//移動（ベクトルを加算）
	worldTransform_.translation_ += speed;

	//発射タイマーカウントダウン
	shotCoolTime--;
	//指定時間に達した
	if (shotCoolTime <= 0) {
		Shot();
		//発射タイマーの初期化
		shotCoolTime = kFireInterval;
	}
	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::phese_ApproachIni()
{
	shotCoolTime = kFireInterval;
}

void Enemy::phase_Leave()
{
	speed = { 0.3f,0,0 };
	//移動（ベクトルを加算）
	worldTransform_.translation_ -= speed;
}

void (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::phase_Approach,
	&Enemy::phase_Leave
};