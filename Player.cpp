
#include "Player.h"

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
	matrix.ChangeTranslation(worldTransform_, 0, 0, 0);
	matrix.UpdateMatrix(worldTransform_);
}

void Player::Update()
{
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
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Player::Draw(ViewProjection viewProjection_)
{
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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
		"player:(%f)", worldTransform_/*[PartID::Root]*/.rotation_.y);
}

void Player::OnCollisioin()
{
}

void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�L�����N�^�[�̈ړ����x
	const float playerSpeed = 0.2f;

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

	//�����_�ړ��i�x�N�g���̉��Z�j
	worldTransform_.translation_ += move;
	
	//�ړ����E���W
	const float moveLimitX = 35;
	const float moveLimitY = 18;
	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
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
	if (input_->TriggerKey(DIK_SPACE)) {
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = transform(velocity, worldTransform_.matWorld_);
		velocity.normalize();
		// �e�����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// �e��o�^
		bullets_.push_back(std::move(newBullet));
		
	}

	//�f�X�t���O�̗����������폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
}