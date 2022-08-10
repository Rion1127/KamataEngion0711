#pragma once
#include "WorldTransform.h"
#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player
{
public:
	//������
	void Initialize(Model* model,uint32_t textureHandle);
	//�X�V
	void Update();
	//���[���h���W���擾
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//�`��
	void Draw(ViewProjection viewProjection_);

	void OnCollisioin();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	void Move();
	void Rotation();
	void Attack();
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	Matrix matrix;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//����
	Input* input_ = nullptr;
	
	DebugText* debugText_ = nullptr;

	enum PartID {
		Root,	//��{
		Spine,	//�Ґ�
		Chest,	//��
		Head,	//��
		ArmL,	//���r
		ArmR,	//�E�r
		Hip,	//�K
		LegL,	//����
		LegR,	//�E��

		endNum,	//�Ō�̔ԍ�
	};

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};

