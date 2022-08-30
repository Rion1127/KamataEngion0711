
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

	//�e�N�X�`���ǂݍ���
	textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	matrix.ScaleChange(worldTransform_, 1, 1, 1, 1);
	matrix.RotaChange(worldTransform_, 0, 0, ConvertAngleToRadian(0));
	matrix.ChangeTranslation(worldTransform_, 0, 15, 0);
	matrix.UpdateMatrix(worldTransform_);
	//�G�t�F�N�g
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


	collisionCoolTime = maxCollisionCoolTime;
	num = 0;
	audio_ = Audio::GetInstance();
	
}

void Enemy2::Update()
{
	if (isAlive == true) {
		//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
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
			//�G�t�F�N�g�̏o�����W���X�V
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
		//���f���̕`��
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
	if (isCollision == true) {
		model_->Draw(effectWorldTransform_, viewProjection, effectTexture);
	}

	
	//�f�o�b�O�\��
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
	//hp��0����̎�
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

#pragma region �t�F�[�Y
void Enemy2::phase_OverTake()
{
	//�v���C���[��ǂ��z��
	Vector3 speed = { 0,0,0.2f };

	worldTransform_.translation_ += speed;

	//����̈ʒu�ɓ��B�����痣�E
	if (player_->GetWorldPosition().z + 50 < worldTransform_.translation_.z) {
		phase_ = Phase::Assault;
		phase_AssaultIni();
	}
}

void Enemy2::phase_AssaultIni()
{
	AssaultVec = player_->GetWorldPosition() - worldTransform_.translation_;
	AssaultVec.z = AssaultVec.z + 15.0f;
	//�v���C���[�ɓˌ�����
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