
#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include <memory>
#include <list>
#include "Player.h"

#include "Enemy2.h"

Enemy2::Enemy2()
{

}

Enemy2::~Enemy2()
{
	
}

void Enemy2::Initialize(Model* model, uint32_t textureHandle)
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
	matrix.RotaChange(worldTransform_, 0, 0, ConvertAngleToRadian(0));
	matrix.ChangeTranslation(worldTransform_, 0, 15, 0);
	matrix.UpdateMatrix(worldTransform_);
	//エフェクト
	effectTexture = TextureManager::Load("white.png");
	effectWorldTransform_.Initialize();
	matrix.ScaleChange(effectWorldTransform_, 1.1f, 1.1f, 1.1f, 1);
	matrix.RotaChange(effectWorldTransform_, worldTransform_);
	matrix.ChangeTranslation(effectWorldTransform_, worldTransform_);
	matrix.UpdateMatrix(effectWorldTransform_);
	effectAliveTime = 5;


	collisionCoolTime = maxCollisionCoolTime;
	num = 0;
}

void Enemy2::Initialize(Model* model, uint32_t textureHandle, Vector3 pos)
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
	matrix.RotaChange(worldTransform_, 0, 0, ConvertAngleToRadian(0));
	matrix.ChangeTranslation(worldTransform_, pos.x, pos.y, pos.z);
	matrix.UpdateMatrix(worldTransform_);
	//エフェクト
	effectTexture = TextureManager::Load("white.png");
	effectWorldTransform_.Initialize();
	matrix.ScaleChange(effectWorldTransform_, 1.1f, 1.1f, 1.1f, 1);
	matrix.RotaChange(effectWorldTransform_, worldTransform_);
	matrix.ChangeTranslation(effectWorldTransform_, worldTransform_);
	matrix.UpdateMatrix(effectWorldTransform_);
	effectAliveTime = 5;


	collisionCoolTime = maxCollisionCoolTime;
	num = 0;
	audio_ = Audio::GetInstance();
	
}

void Enemy2::Update()
{
	if (isAlive == true) {
		//メンバ関数ポインタに入っている関数を呼び出す
		(this->*spFuncTable[static_cast<size_t>(phase_)])();

		matrix.UpdateMatrix(worldTransform_);

		if (hp <= 0) {
			isDead = true;
			isAlive = false;
		}

		if (player_->GetWorldPosition().z > worldTransform_.translation_.z + 50) {
			isDead = true;
		}

		if (isCollision == true) {
			//エフェクトの出現座標を更新
			matrix.RotaChange(effectWorldTransform_, worldTransform_);
			matrix.ChangeTranslation(effectWorldTransform_, worldTransform_);
			matrix.UpdateMatrix(effectWorldTransform_);
		}
	}
}



Vector3 Enemy2::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Enemy2::Draw(const ViewProjection& viewProjection)
{
	if (isAlive == true) {
		//モデルの描画
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
	if (isCollision == true) {
		model_->Draw(effectWorldTransform_, viewProjection, effectTexture);
	}

	
	//デバッグ表示
	debugText_->SetPos(50, 190);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	//debugText_->SetPos(50, 210);
	//debugText_->Printf(
	//	"hp:%d",
	//	hp);
}

void Enemy2::CollisionCooltime()
{
	collisionCoolTime--;

	if (isCollision == true) {
		effectAliveTime--;
		if (effectAliveTime <= 0) {
			isCollision = false;
			effectAliveTime = 5;
		}
	}
}

void Enemy2::OnCollisioin(uint32_t hitSE)
{
	//hpが0より上の時
	if (hp > 0) {
		if (collisionCoolTime <= 0) {
			audio_->PlayWave(hitSE, false, 1.0f);
			num++;
			hp--;
			isCollision = true;
			collisionCoolTime = maxCollisionCoolTime;
		}
	}
}

#pragma region フェーズ
void Enemy2::phase_OverTake()
{
	//プレイヤーを追い越す
	Vector3 speed = { 0,0,0.2f };

	worldTransform_.translation_ += speed;

	//既定の位置に到達したら離脱
	if (player_->GetWorldPosition().z + 50 < worldTransform_.translation_.z) {
		phase_ = Phase::Assault;
		phase_AssaultIni();
	}
}

void Enemy2::phase_AssaultIni()
{
	AssaultVec = player_->GetWorldPosition() - worldTransform_.translation_;
	AssaultVec.z = AssaultVec.z + 15.0f;
	//プレイヤーに突撃する
	float speed = 0.2f;

	AssaultVec.normalize();
	AssaultVec *= speed;
}

void Enemy2::phase_Assault()
{
	worldTransform_.AddPosition(AssaultVec);
}

void (Enemy2::* Enemy2::spFuncTable[])() = {
	&Enemy2::phase_OverTake,
	&Enemy2::phase_Assault
};
#pragma endregion