#include "MathUtility.h"
#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = textureHandle;
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, 0, 0);
	matrix.UpdateMatrix(worldTransform_);
}

void Player::Update()
{
	Move();
	Rotation();
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//行列更新
	matrix.UpdateMatrix(worldTransform_);
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Player::Draw(ViewProjection viewProjection_)
{
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	//デバッグ表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"player:(%f,%f,%f)", worldTransform_/*[PartID::Root]*/.translation_.x,
		worldTransform_/*[PartID::Root]*/.translation_.y,
		worldTransform_/*[PartID::Root]*/.translation_.z);
	//デバッグ表示
	debugText_->SetPos(50, 170);
	debugText_->Printf(
		"player:(%f)", worldTransform_/*[PartID::Root]*/.rotation_.y);
	//デバッグ表示
	debugText_->SetPos(50, 410);
	debugText_->Printf(
		"timeRate:(%f)", timeRate);
	//デバッグ表示
	debugText_->SetPos(50, 430);
	debugText_->Printf(
		"nowtime:(%f)", nowtime);
}

void Player::OnCollisioin()
{
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//キャラクターの移動速度
	const float playerSpeed = 0.2f;


	Vector3 start(0, 0, 0);
	Vector3 p1(0, 10, 0);
	Vector3 p2(0, -10, 0);
	Vector3 end(0, 0, 50);
	float maxTime = 5.0f;

	/*std::vector<Vector3> point{ start,start,p1,p2,end,end };
	if (timeRate >= 1.0f) {
		if (startIndex < point.size() - 3) {
			startIndex++;
			timeRate -= 1.0f;
			nowtime = 0;
		}
		else {
			timeRate = 1.0f;
		}
	}*/
	
	timeRate++;
	//timeRate / FPS　で1秒のカウントをnowTimeに代入する 
	nowtime = timeRate / 120;

	nowtime = min(nowtime / maxTime, 1.0f);

	Vector3 a = lerp(start, p1, nowtime);
	Vector3 b = lerp(p1, end, nowtime);

	position = ease_in_out(a, b, nowtime);



#pragma region
	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_A)) {
	//	move = { -playerSpeed,0,0 };
	//}
	//else if (input_->PushKey(DIK_D)) {
	//	move = { playerSpeed,0,0 };
	//}
	//if (input_->PushKey(DIK_W)) {
	//	move = { 0,playerSpeed,0 };
	//}
	//else if (input_->PushKey(DIK_S)) {
	//	move = { 0,-playerSpeed,0 };
	//}

	//注視点移動（ベクトルの加算）
	worldTransform_.translation_ = position;
	
	//移動限界座標
	const float moveLimitX = 35;
	const float moveLimitY = 18;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
#pragma endregion
}

void Player::Rotation()
{
	const float rotaSpeed = 0.01f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= rotaSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += rotaSpeed;
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = transform(velocity, worldTransform_.matWorld_);
		velocity.normalize();
		// 弾生成、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録
		bullets_.push_back(std::move(newBullet));
		
	}

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
}
