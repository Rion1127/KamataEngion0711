#pragma once
#include "WorldTransform.h"
#include "EnemyBullet.h"


//���@�N���X�̑O���錾
class Player;

class Enemy
{
public:
	
	Enemy();
	~Enemy();
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
	//�X�V
	void Update();

	void Shot();
	bool GetisAlive() { return isAlive; }
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	void SetPlayer(Player* player) { player_ = player; }
	//�`��
	void Draw(const ViewProjection& viewProjection);

	void CollisionCooltime();
	void OnCollisioin();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//����
	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Model* bulletModel = nullptr;

	//HP
	bool isAlive = true;
	const int maxHp = 7;
	int hp = maxHp;
	bool isCollision = false;

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
	static const int kFireInterval = 360;
	int32_t shotCoolTime = 0;

	Player* player_ = nullptr;

	const int maxCollisionCoolTime = 20;
	int collisionCoolTime;
	int num;

	//�G�t�F�N�g
	uint32_t effectTexture;
	WorldTransform effectWorldTransform_;
	int effectAliveTime;
};
