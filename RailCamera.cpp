#include "RailCamera.h"

void RailCamera::Ini(Vector3 pos, Vector3 rot)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform.translation_ = pos;
	worldTransform.rotation_ = rot;
	worldTransform.CreateConstBuffer();
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection.Initialize();

	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
	float speed = -0.05f;

	worldTransform.translation_ += { 0,0,-speed };
	worldTransform.rotation_ += { 0,0,0 };
	//worldTransform.TransferMatrix();

	viewProjection.eye = worldTransform.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = transform(forward, worldTransform.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection.target = viewProjection.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f�i���[���J�����̏���x�N�g���j
	//viewProjection.up = transform(up, worldTransform.matWorld_);
	viewProjection.UpdateMatrix();
	viewProjection.TransferMatrix();

	//�f�o�b�O�\��
	debugText_->SetPos(50, 250);
	debugText_->Printf(
		"viewProjection.eye:(%f,%f,%f)", viewProjection.eye.x,
		viewProjection.eye.y,
		viewProjection.eye.z);
	debugText_->SetPos(50, 270);
	debugText_->Printf(
		"viewProjection.up:(%f,%f,%f)", viewProjection.up.x,
		viewProjection.up.y,
		viewProjection.up.z);
	debugText_->SetPos(50, 290);
	debugText_->Printf(
		"viewProjection.target:(%f,%f,%f)", viewProjection.target.x,
		viewProjection.target.y,
		viewProjection.target.z);
}
