#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, 0);
	worldTransform_.translation_ = position;
	matrix.UpdateMatrix(worldTransform_);

	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	//���W���ړ�������i1�t���[�����̈ړ��ʂ𑫂����ށj
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	matrix.UpdateMatrix(worldTransform_);

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollisioin()
{
	isDead_ = true;
}
