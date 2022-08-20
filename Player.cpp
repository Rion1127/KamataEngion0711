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

	//�t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
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
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�s��X�V
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
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	//�f�o�b�O�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"player:(%f,%f,%f)", worldTransform_/*[PartID::Root]*/.translation_.x,
		worldTransform_/*[PartID::Root]*/.translation_.y,
		worldTransform_/*[PartID::Root]*/.translation_.z);
	//�f�o�b�O�\��
	debugText_->SetPos(50, 170);
	debugText_->Printf(
		"startIndex:(%d)", startIndex);
	//�f�o�b�O�\��
	debugText_->SetPos(50, 410);
	debugText_->Printf(
		"timeRate:(%f)", time / 120);
	//�f�o�b�O�\��
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
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�L�����N�^�[�̈ړ����x
	const float playerSpeed = 0.2f;


	float maxTime = 5.0f;

	//if (timeRate >= 1.0f) {
	//	//���̐���_������ꍇ
	//	if (startIndex < points.size() - 3) {
	//		startIndex++;
	//		time = 0.0f;
	//		timeRate = 0;
	//	}
	//	//�ŏI�n�_�������ꍇ1.0f�ɂ��ē������~�߂�
	//	else {
	//		timeRate = 1.0f;
	//	}
	//}
	//
	//time++;
	////timeRate / FPS�@��1�b�̃J�E���g��nowTime�ɑ������ 
	//timeRate = time / 120;
	//timeRate = min(timeRate / maxTime, 1.0f);
	////�x�W�G�Ȑ�
	//*Vector3 a = lerp(start, p1, nowtime);
	//Vector3 b = lerp(p1, end, nowtime);
	//position = ease_in_out(a, b, nowtime);*/
	//position = SplinePosition(points, startIndex, timeRate);

	

#pragma region �ړ�
	//�����������ňړ��x�N�g����ύX
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

	//L�X�e�B�b�N�ňړ�����
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

	//�����_�ړ��i�x�N�g���̉��Z�j
	//worldTransform_.translation_ = position;

	worldTransform_.translation_ += move;
	worldTransform_.translation_ += speed;
	
	//�ړ����E���W
	const float moveLimitX = 18;
	const float moveLimitY = 9;
	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
#pragma endregion

#pragma region ��]
	Vector3 rotSpeed(0, 0, 0);
	//���X�ɉ�]������
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

	//���[�e�[�V���������X��0�ɖ߂�
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

	//���E��]��
	const float rotLimitX = ConvertAngleToRadian(20.0f);
	const float rotLimitY = ConvertAngleToRadian(20.0f);
	const float rotLimitZ = ConvertAngleToRadian(20.0f);
	//�ړ��ʐ���
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
	//�����������ňړ��x�N�g����ύX
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

			// �e�̑��x
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
			velocity = transform(velocity, worldTransform_.matWorld_);
			velocity.normalize();
			// �e�����A������
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel, pos, velocity);

			// �e��o�^
			bullets_.push_back(std::move(newBullet));

			cooltime = 5;
		}
	}

	//�f�X�t���O�̗����������폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
}
