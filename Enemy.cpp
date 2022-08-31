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

Enemy::~Enemy()
{
	delete bulletModel;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//�e�N�X�`���ǂݍ���
	textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0,ConvertAngleToRadian(10));
	matrix.ChangeTranslation(worldTransform_, 0, 3, 50);
	matrix.UpdateMatrix(worldTransform_);
	//�G�t�F�N�g
	effectTexture = TextureManager::Load("white.png");
	effectWorldTransform_.Initialize();
	matrix.ScaleChange(effectWorldTransform_, 1.1f, 1.1f, 1.1f, 1);
	matrix.RotaChange(effectWorldTransform_, worldTransform_);
	matrix.ChangeTranslation(effectWorldTransform_,worldTransform_);
	matrix.UpdateMatrix(effectWorldTransform_);
	effectAliveTime = 5;

	//Shot();
	bulletModel = Model::Create();

	phese_ApproachIni();

	collisionCoolTime = maxCollisionCoolTime;
	num = 0;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 pos)
{
	assert(model);

	//�e�N�X�`���ǂݍ���
	textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, ConvertAngleToRadian(0));
	matrix.ChangeTranslation(worldTransform_, pos.x, pos.y, pos.z);
	matrix.UpdateMatrix(worldTransform_);
	//�G�t�F�N�g
	effectTexture = TextureManager::Load("white.png");
	effectWorldTransform_.Initialize();
	matrix.ScaleChange(effectWorldTransform_, 1.1f, 1.1f, 1.1f, 1);
	matrix.RotaChange(effectWorldTransform_, worldTransform_);
	matrix.ChangeTranslation(effectWorldTransform_, worldTransform_);
	matrix.UpdateMatrix(effectWorldTransform_);
	effectAliveTime = 5;

	//Shot();
	bulletModel = Model::Create();

	phese_ApproachIni();

	collisionCoolTime = maxCollisionCoolTime;
	num = 0;
	audio_ = Audio::GetInstance();
	hitSE = audio_->LoadWave("hitSE.wav");
}

void Enemy::Update()
{
	if (isAlive == true) {
		//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
		(this->*spFuncTable[static_cast<size_t>(phase_)])();

		matrix.UpdateMatrix(worldTransform_);
		if (bullets_.size() <= 0) {
			if (hp <= 0) {
				isDead = true;
			}

			if (player_->GetWorldPosition().z - 50 > worldTransform_.translation_.z) {
				isDead = true;
			}
		}
		if (hp <= 0) {
			isAlive = false;
		}

		if (isCollision == true) {
			//�G�t�F�N�g�̏o�����W���X�V
			matrix.RotaChange(effectWorldTransform_, worldTransform_);
			matrix.ChangeTranslation(effectWorldTransform_, worldTransform_);
			matrix.UpdateMatrix(effectWorldTransform_);
		}
	}
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	
}

void Enemy::Shot()
{
	assert(player_);

	// �e�̑��x
	const float kBulletSpped = 0.5f;

	// �����x�N�g�����i�[����ϐ�
	Vector3 diffVec;

	// ���L�����A�G�L�����̃��[���h���W���擾
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = worldTransform_.translation_;

	// �����x�N�g�������߂�
	diffVec = enemyPos - playerPos;

	// �����x�N�g���̐��K��
	diffVec.normalize();

	// �x�N�g���̒������A�����ɍ��킹��B
	diffVec *= kBulletSpped;

	// �e�̑��x
	
	Vector3 velocity2(0, 0, kBulletSpped);

	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity2 = transform(velocity2, worldTransform_.matWorld_);
	velocity2.normalize();
	velocity2 *= kBulletSpped;

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bulletModel, worldTransform_.translation_, worldTransform_.rotation_, velocity2);
	// �e��o�^����
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
	if (isAlive == true) {
		//���f���̕`��
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
	if (isCollision == true) {
		model_->Draw(effectWorldTransform_, viewProjection, effectTexture);
	}

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	////�f�o�b�O�\��
	/*debugText_->SetPos(50, 190);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);*/

	//debugText_->SetPos(50, 210);
	//debugText_->Printf(
	//	"hp:%d",
	//	hp);
}

void Enemy::CollisionCooltime()
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

void Enemy::OnCollisioin(uint32_t hitSE)
{
	
	//hp��0����̎�
	if (hp > 0) {
		if (collisionCoolTime <= 0) {
			audio_->PlayWave(hitSE,false,1.0f);
			num++;
			hp--;
			isCollision = true;
			collisionCoolTime = maxCollisionCoolTime;
		}
	}
}

#pragma region �t�F�[�Y
void Enemy::phase_Approach()
{
	speed = { 0,0,-0.005f };
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_ += speed;

	//���˃^�C�}�[�J�E���g�_�E��
	shotCoolTime--;
	//�w�莞�ԂɒB����
	if (shotCoolTime <= 0) {
		Shot();
		//���˃^�C�}�[�̏�����
		shotCoolTime = kFireInterval;
	}
	//�f�X�t���O�̗����������폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//����̈ʒu�ɓ��B�����痣�E
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
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_ -= speed;
}

void (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::phase_Approach,
	&Enemy::phase_Leave
};
#pragma endregion