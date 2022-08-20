#include "MathUtility.h"
#include "Player.h"

Player::~Player()
{
	delete bulletModel;
	delete model_;
	
}

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
	matrix.ChangeTranslation(worldTransform_, 0, 0, 25);
	matrix.UpdateMatrix(worldTransform_);

	pad.Ini();

	bulletModel = Model::Create();
	cooltime = 10;
}

void Player::Update()
{
	pad.Update();
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
	Vector3 worldPos =
	{ worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2] };
	return worldPos;
}

void Player::Draw(ViewProjection viewProjection_)
{
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection_);

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
		"startIndex:(%d)", startIndex);
	//デバッグ表示
	debugText_->SetPos(50, 410);
	debugText_->Printf(
		"timeRate:(%f)", time / 120);
	//デバッグ表示
	debugText_->SetPos(50, 430);
	debugText_->Printf(
		"timeRate:(%f)", timeRate);
}

void Player::OnCollisioin()
{
}

void Player::SetParent(WorldTransform& worldTransform)
{
	worldTransform_.parent_ = &worldTransform;
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//キャラクターの移動速度
	const float playerSpeed = 0.2f;


	float maxTime = 5.0f;

	//if (timeRate >= 1.0f) {
	//	//次の制御点がある場合
	//	if (startIndex < points.size() - 3) {
	//		startIndex++;
	//		time = 0.0f;
	//		timeRate = 0;
	//	}
	//	//最終地点だった場合1.0fにして動きを止める
	//	else {
	//		timeRate = 1.0f;
	//	}
	//}
	//
	//time++;
	////timeRate / FPS　で1秒のカウントをnowTimeに代入する 
	//timeRate = time / 120;
	//timeRate = min(timeRate / maxTime, 1.0f);
	////ベジエ曲線
	//*Vector3 a = lerp(start, p1, nowtime);
	//Vector3 b = lerp(p1, end, nowtime);
	//position = ease_in_out(a, b, nowtime);*/
	//position = SplinePosition(points, startIndex, timeRate);

	

#pragma region 移動
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		move = { -playerSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { playerSpeed,0,0 };
	}
	if (input_->PushKey(DIK_W)) {
		move = { 0,playerSpeed,0 };
	}
	else if (input_->PushKey(DIK_S)) {
		move = { 0,-playerSpeed,0 };
	}

	//Lスティックで移動する
	Vector2 speed;
	speed.x += pad.GetLStick().x * 0.000005f;
	speed.y -= pad.GetLStick().y * 0.000005f;
	speed.y *= 0.6f;

	if (speed.x >= 0.6f) {
		speed.x = 0.6f;
	}
	else if (speed.x <= -0.6f) {
		speed.x = -0.6f;
	}

	if (speed.y >= 0.6f) {
		speed.y = 0.6f;
	}
	else if (speed.y <= -0.6f) {
		speed.y = -0.6f;
	}

	//注視点移動（ベクトルの加算）
	//worldTransform_.translation_ = position;

	worldTransform_.translation_ += move;
	worldTransform_.translation_ += speed;
	
	//移動限界座標
	const float moveLimitX = 18;
	const float moveLimitY = 9;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
#pragma endregion

#pragma region 回転
	Vector3 rotSpeed(0, 0, 0);
	//徐々に回転させる
	if (speed.x > 0) {
		rotSpeed.y = ConvertAngleToRadian(speed.x * 3.5f);
		rotSpeed.z = ConvertAngleToRadian(-speed.x * 3.5f);
	}
	else if (speed.x < 0) {
		rotSpeed.y = ConvertAngleToRadian(speed.x * 3.5f);
		rotSpeed.z = ConvertAngleToRadian(-speed.x * 3.5f);
	}

	if (speed.y > 0) {
		rotSpeed.x = -ConvertAngleToRadian(speed.y * 3.5f);
	}
	else if (speed.y < 0) {
		rotSpeed.x = -ConvertAngleToRadian(speed.y * 3.5f);
	}

	Vector2 hikaku(0, 0);

	//ローテーションを徐々に0に戻す
	if (speed.x == 0) {
		if (worldTransform_.rotation_.y < 0) {
			worldTransform_.AddRotation(0, ConvertAngleToRadian(1.0f), 0);
		}
		else if (worldTransform_.rotation_.y > 0) {
			worldTransform_.AddRotation(0, -ConvertAngleToRadian(1.0f), 0);
		}

		if (worldTransform_.rotation_.z < 0) {
			worldTransform_.AddRotation(0, 0, ConvertAngleToRadian(1.0f));
		}
		else if (worldTransform_.rotation_.z > 0) {
			worldTransform_.AddRotation(0, 0, -ConvertAngleToRadian(1.0f));
		}

		if (worldTransform_.rotation_.y <= ConvertAngleToRadian(1.0f) &&
			worldTransform_.rotation_.y >= -ConvertAngleToRadian(1.0f) &&
			worldTransform_.rotation_.y != 0)
		{
			worldTransform_.rotation_.y = 0;
		}

		if (worldTransform_.rotation_.z <= ConvertAngleToRadian(1.0f) &&
			worldTransform_.rotation_.z >= -ConvertAngleToRadian(1.0f) &&
			worldTransform_.rotation_.z != 0)
		{
			worldTransform_.rotation_.z = 0;
		}
	}

	if (speed.y == 0) {
		if (worldTransform_.rotation_.x < 0) {
			worldTransform_.AddRotation(ConvertAngleToRadian(1.0f), 0, 0);
		}
		else if (worldTransform_.rotation_.x > 0) {
			worldTransform_.AddRotation(-ConvertAngleToRadian(1.0f), 0, 0);
		}

		if (worldTransform_.rotation_.x <= ConvertAngleToRadian(1.0f) &&
			worldTransform_.rotation_.x >= -ConvertAngleToRadian(1.0f) &&
			worldTransform_.rotation_.x != 0)
		{
			worldTransform_.rotation_.x = 0;
		}
	}

	//限界回転量
	const float rotLimitX = ConvertAngleToRadian(20.0f);
	const float rotLimitY = ConvertAngleToRadian(20.0f);
	const float rotLimitZ = ConvertAngleToRadian(20.0f);
	//移動量制限
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x, -rotLimitX);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x, +rotLimitX);
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -rotLimitY);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +rotLimitY);
	worldTransform_.rotation_.z = max(worldTransform_.rotation_.z, -rotLimitZ);
	worldTransform_.rotation_.z = min(worldTransform_.rotation_.z, +rotLimitZ);

	worldTransform_.AddRotation(rotSpeed.x, rotSpeed.y, rotSpeed.z);

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
	cooltime--;
	if (cooltime <= 0) {
		if (input_->TriggerKey(DIK_SPACE) || pad.GetButtons(XINPUT_GAMEPAD_B)) {
			Vector3 pos =
			{ worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2] };

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = transform(velocity, worldTransform_.matWorld_);
			velocity.normalize();
			// 弾生成、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel, pos, velocity);

			// 弾を登録
			bullets_.push_back(std::move(newBullet));

			cooltime = 5;
		}
	}

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
}
