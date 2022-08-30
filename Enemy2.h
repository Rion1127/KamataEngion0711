#pragma once
#pragma once
#include "WorldTransform.h"


//���@�N���X�̑O���錾
class Player;

class Enemy2
{
public:

	Enemy2();
	~Enemy2();
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
	//�X�V
	void Update();


	bool IsDead() { return isDead; }
	bool IsAlive() { return  isAlive; }
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	void SetPlayer(Player* player) { player_ = player; }
	//�`��
	void Draw(const ViewProjection& viewProjection);

	void CollisionCooltime();
	void OnCollisioin(uint32_t hitSE);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//����
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	DebugText* debugText_ = nullptr;

	//HP
	bool isAlive = true;
	bool isDead = false;
	const int maxHp = 3;
	int hp = maxHp;
	bool isCollision = false;

	//�s���t�F�[�Y
	enum class Phase {
		OverTake,	//�ڋ߂���
		Assault,		//���E����
	};

	//�t�F�[�Y
	Phase phase_ = Phase::OverTake;

	Vector3 speed = { 0,0,-0.1f };

	void phase_OverTake();
	void phase_AssaultIni();
	void phase_Assault();
	Vector3 AssaultVec;

	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy2::* spFuncTable[])();

	Player* player_ = nullptr;

	const int maxCollisionCoolTime = 20;
	int collisionCoolTime;
	int num;

	//�G�t�F�N�g
	uint32_t effectTexture;
	WorldTransform effectWorldTransform_;
	int effectAliveTime;
	
};


