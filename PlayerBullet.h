#pragma once
#include "Matrix.h"
#include <cassert>

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
class PlayerBullet
{
public:
	~PlayerBullet();
	//������
	void Initialize(Model* model, const Vector3& position, const Vector3 rot, const Vector3& velocity);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	WorldTransform GetWorldTransform() { return worldTransform_; }
	Vector3 GetWorldPosition() { return worldTransform_.translation_; }
	bool IsDead() const { return isDead_; }


	void OnCollisioin();

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//����
	static const int32_t lifeTime = 60 * 2;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = lifeTime;
	//�f�X�t���O
	bool isDead_ = false;

	//���x
	Vector3 velocity_;
};

