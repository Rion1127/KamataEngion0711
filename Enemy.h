#pragma once
#include "WorldTransform.h"
#include "EnemyBullet.h"

//���@�N���X�̑O���錾
class Player;

class Enemy
{
public:
	
	Enemy();
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	//�X�V
	void Update();

	void Shot();
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
	//�`��
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//����
	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//�s���t�F�[�Y
	enum class Phase {
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	
	Vector3 speed = { 0,0,-0.1f };

	void phase_Approach();
	void phese_ApproachIni();
	void phase_Leave();

	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy::* spFuncTable[])();

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//ShotTimer
	static const int kFireInterval = 60;
	int32_t shotCoolTime = 0;

	Player* player_ = nullptr;

};
