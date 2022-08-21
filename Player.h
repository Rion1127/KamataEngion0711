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
	~Player();
	//������
	void Initialize(Model* model,uint32_t textureHandle);
	//�X�V
	void Update();
	//���[���h���W���擾
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//�`��
	void Draw(ViewProjection viewProjection_);
	void DrawReticle();

	void CollisionCooltime();
	void OnCollisioin();

	void SetParent(WorldTransform& worldTransform);
	void Get2DReticlePosition(ViewProjection viewProjection);

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
	Model* bulletModel = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//����
	Input* input_ = nullptr;
	Controller pad;
	
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
	int cooltime;


	Vector3 position;
	float time = 0;
	float timeRate = 0;
	
	

	Vector3 start = { 0, 0, 0 };
	Vector3 p2 = { 20,10,10 };
	Vector3 p3 = { -10,-10,20 };
	Vector3 end = { 0,15,30 };
	std::vector<Vector3> points{ start,start,p2,p3,end,end };
	//p1����X�^�[�g����
	size_t startIndex = 1;

	const int maxCollisionCoolTime = 20;
	int collisionCoolTime = maxCollisionCoolTime;

	//���e�B�N��
	Model* ReticleModel;
	WorldTransform reticlePosition;
	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr <Sprite> sprite2Dreticle;
};

