#include "MathUtility.h"
#include "WinApp.h"
#include <random>
#include "Player.h"

Player::~Player()
{
	delete bulletModel;
	delete model_;
	delete ReticleModel;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = textureHandle;
	model_ = model;
	bulletModel = Model::Create();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, -10, 20);
	matrix.UpdateMatrix(worldTransform_);

	pad.Ini();

	cooltime = 10;

	collisionCoolTime = maxCollisionCoolTime;
	//照準の初期化
	reticlePosition.Initialize();
	ReticleModel = Model::Create();
	matrix.ScaleChange(reticlePosition, 1, 1, 1, 1);
	matrix.RotaChange(reticlePosition, 0, 0, 0);
	matrix.ChangeTranslation(reticlePosition, 0, 0, 0);
	matrix.UpdateMatrix(reticlePosition);
	//レティクルの画像
	uint32_t reticleTexture = TextureManager::Load("reticle.png");
	Vector2 pos = { WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 };
	Vector2 mid = { 0.5f,0.5f };
	sprite2Dreticle.reset(Sprite::Create(reticleTexture, pos, Vector4(1, 1, 1, 1), mid));

	uint32_t hpBarTexture = TextureManager::Load("hpBar2.png");
	Vector2 hpPos = {
		WinApp::kWindowWidth / 2 + 450,
		WinApp::kWindowWidth / 2 - 70 };
	Vector2 hpBarMid = { 0.5f,0.5f };
	sprite2HpBar.reset(Sprite::Create(hpBarTexture, hpPos, Vector4(1, 1, 1, 1), hpBarMid));

	/*engineParticleTF.Initialize();
	engineParticleTF.parent_ = &worldTransform_;
	matrix.ScaleChange(engineParticleTF,1, 1, 1,1);
	matrix.RotaChange(engineParticleTF, 0, 0, 0);
	matrix.ChangeTranslation(engineParticleTF, 0, 0, -5);
	matrix.UpdateMatrix(engineParticleTF);*/
	//particleModel = Model::Create();

	audio_ = Audio::GetInstance();
	shotSE = audio_->LoadWave("shot.wav");
}

void Player::Reset()
{
	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	matrix.ChangeTranslation(worldTransform_, 0, -10, 20);
	matrix.UpdateMatrix(worldTransform_);
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

	for (std::unique_ptr<Particle>& particle_ : particle_) {
		particle_->Update();
	}

	//パーティクル生成、初期化

	if (particle_.size() < 10) {
		std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
		newParticle->Ini(worldTransform_);

		// 弾を登録
		particle_.push_back(std::move(newParticle));
	}
	particleCooltime = maxCollisionCoolTime;

	//デスフラグの立った球を削除
	particle_.remove_if([](std::unique_ptr<Particle>& particle) {
		return particle->GetDead();
		});
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos =
	{
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2]
	};
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

	//弾描画
	for (std::unique_ptr<Particle>& particle_ : particle_) {
		particle_->Draw(viewProjection_);

	}

	//ReticleModel->Draw(reticlePosition, viewProjection_);

	//デバッグ表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"player:(%f,%f,%f)", GetWorldPosition().x,
		GetWorldPosition().y,
		GetWorldPosition().z);
	////デバッグ表示
	//debugText_->SetPos(50, 170);
	//debugText_->Printf(
	//	"startIndex:(%d)", startIndex);
	////デバッグ表示
	//debugText_->SetPos(50, 410);
	//debugText_->Printf(
	//	"timeRate:(%f)", time / 120);
	////デバッグ表示
	//debugText_->SetPos(50, 430);
	//debugText_->Printf(
	//	"reticlePosition:(%f,%f,%f)",
	//	reticlePosition.GetWorldPosition().x,
	//	reticlePosition.GetWorldPosition().y,
	//	reticlePosition.GetWorldPosition().z);
}

void Player::DrawUI()
{
	sprite2Dreticle->Draw();
	sprite2HpBar->Draw();
}

void Player::CollisionCooltime()
{
	collisionCoolTime--;
}

void Player::OnCollisioin()
{

	if (collisionCoolTime <= 0) {

		collisionCoolTime = maxCollisionCoolTime;
	}
}

void Player::SetParent(WorldTransform& worldTransform)
{
	worldTransform_.parent_ = &worldTransform;
}

void Player::Get2DReticlePosition(ViewProjection viewProjection)
{
	const float kDistance = 10.0f;
	Vector3 offset = { 0,0,1.0f };
	offset = /*MathUtility::*/Vector3toTransform(offset, worldTransform_.matWorld_);
	offset = offset.normalize() * kDistance;
	reticlePosition.translation_ = worldTransform_.GetWorldPosition() + offset;
	matrix.UpdateMatrix(reticlePosition);

	/*Vector3 reticle2DPosition = reticlePosition.GetWorldPosition();*/

	Vector4 vec4Reti2Dpos = {
		reticlePosition.GetWorldPosition().x * 1.7f,
		reticlePosition.GetWorldPosition().y * 1.4f,
		5,
		1
	};
	//ビューポート行列
	// ビューポート行列（スクリーン行列）の作成
	float w = WinApp::kWindowWidth / 2.0f;
	float h = WinApp::kWindowHeight / 2.0f;

	Matrix4 matViewport = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w - reticlePosition.GetWorldPosition().x  , h + reticlePosition.GetWorldPosition().y  , 0 , 1
	};

	/*reticle2DPosition = MathUtility::Vector3Transform(reticle2DPosition, viewProjection.matView);
	reticle2DPosition = MathUtility::Vector3Transform(reticle2DPosition, viewProjection.matProjection);
	reticle2DPosition = MathUtility::Vector3Transform(reticle2DPosition, matViewport);*/

	vec4Reti2Dpos = B(vec4Reti2Dpos, viewProjection.matView);
	vec4Reti2Dpos = B(vec4Reti2Dpos, viewProjection.matProjection);
	vec4Reti2Dpos = B(vec4Reti2Dpos, matViewport);
	vec4Reti2Dpos = W(vec4Reti2Dpos);

	/*Vector4 vec = { -2,2,0,1 };

	Matrix4 mat1 = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	Matrix4 mat2 = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,3,1
	};
	Matrix4 mat3 = {
		0.75f,0, 0,0,
			0,1, 0,0,
			0,0, 1,1,
			0,0,-1,0
	};
	Matrix4 mat4 = {
		320,   0, 0,0,
		  0,-240, 0,0,
		  0,   0, 1,0,
		320, 240, 0,1
	};
	vec = B(vec, mat1);

	vec = B(vec, mat2);

	vec = B(vec, mat3);
	vec = B(vec, mat4);
	vec = W(vec);*/

	////ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4 matViewProjectionViewPort = viewProjection.matView;
	//matViewProjectionViewPort *= viewProjection.matProjection;
	//matViewProjectionViewPort *= matViewport;

	////ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	//
	//reticle2DPosition = /*MathUtility::Vector3TransformCoord*/ConvertWorldToScreen(reticle2DPosition, matViewProjectionViewPort);
	//スプライトのレティクルに座標設定
	sprite2Dreticle->SetPosition(Vector2(vec4Reti2Dpos.x, vec4Reti2Dpos.y));
	////デバッグ表示
	//debugText_->SetPos(50, 450);
	//debugText_->Printf(
	//	"vec4Reti2Dpos:(%f,%f)",
	//	vec4Reti2Dpos.x,
	//	vec4Reti2Dpos.y);
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
	speed.x += pad.GetLStick().x * 0.000003f;
	speed.y -= pad.GetLStick().y * 0.000003f;
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
	const float moveLimitX = 10;
	const float moveLimitY = 4;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY - 5);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY - 1);
#pragma endregion

#pragma region 回転
	Vector3 rotSpeed(0, 0, 0);
	float rotAdjusment = 4.5f;
	//徐々に回転させる
	if (speed.x > 0) {
		rotSpeed.y = ConvertAngleToRadian(speed.x * rotAdjusment);
		rotSpeed.z = ConvertAngleToRadian(-speed.x * rotAdjusment);
	}
	else if (speed.x < 0) {
		rotSpeed.y = ConvertAngleToRadian(speed.x * rotAdjusment);
		rotSpeed.z = ConvertAngleToRadian(-speed.x * rotAdjusment);
	}

	if (speed.y > 0) {
		rotSpeed.x = -ConvertAngleToRadian(speed.y * rotAdjusment);
	}
	else if (speed.y < 0) {
		rotSpeed.x = -ConvertAngleToRadian(speed.y * rotAdjusment);
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
			audio_->PlayWave(shotSE);
			Vector3 pos =
			{ worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2] };

			// 弾の速度
			const float kBulletSpeed = 3.5f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = transform(velocity, worldTransform_.matWorld_);
			velocity.normalize();
			velocity *= kBulletSpeed;

			// 弾生成、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel, pos, worldTransform_.rotation_, velocity);

			// 弾を登録
			bullets_.push_back(std::move(newBullet));

			cooltime = 15;
		}
	}

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
}


