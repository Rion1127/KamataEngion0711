#include "MathUtility.h"
#include "RailCamera.h"

RailCamera::~RailCamera()
{
	
}

void RailCamera::Ini(Vector3 pos, Vector3 rot)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform.translation_ = pos;
	worldTransform.rotation_ = rot;
	
	worldTransform.Initialize();
	matrix.UnitMatrix(worldTransform.matWorld_);
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection.Initialize();

	debugText_ = DebugText::GetInstance();

	controlPoints_ = {
		{0,0,0},
		{10,10,10},
		{10,15,20},
		{20,15,30},
		{20,0,40},
		{30,0,50},
	};
	//���C���`�悪�Q�Ƃ���r���[�v���W�F�N�V�������w�肷��i�A�h���X�n���j
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
	
	for (int i = 0; i < railPoints.size() - 3; i++) {
		for (int j = 0; j < 100; j++) {
			Vector3 pos = SplinePosition(railPoints, startIndex, t);
			positions[i][j] = pos + Vector3(1,-1,5);
			t += 0.01f;
		}
		//���̐���_�Ɉړ�����
		startIndex += 1;
		//���Z�b�g
		t = 0;
	}
	startIndex = 1;
}

void RailCamera::Update()
{
	//���[���J�����̋���
	Move();

	/*float speed = -0.05f;
	worldTransform.scale_ = { 1,1,1 };
	worldTransform.translation_ += { 0,0,-speed };*/
	
	matrix.UpdateMatrix(worldTransform);

	viewProjection.eye = worldTransform.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, -0.2f, 1);
	//���[���J�����̉�]�𔽉f
	forward = transform(forward, worldTransform.matWorld_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection.target = viewProjection.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f�i���[���J�����̏���x�N�g���j
	viewProjection.up = transform(up, worldTransform.matWorld_);
	viewProjection.UpdateMatrix();
	viewProjection.TransferMatrix();

	//�f�o�b�O�\��
	/*debugText_->SetPos(50, 270);
	debugText_->Printf(
		"time:(%f)", time);
	debugText_->SetPos(50, 290);
	debugText_->Printf(
		"timeRate:(%f)", timeRate);
	debugText_->SetPos(50, 310);
	debugText_->Printf(
		"time / 120:(%f)", time / 60);*/
}

void RailCamera::Move()
{
	float maxTime = 5.0f;

	if (timeRate >= 1.0f) {
		//���̐���_������ꍇ
		if (startIndex < points.size() - 3) {
			startIndex++;
			time = 0.0f;
			timeRate = 0;
		}
		//�ŏI�n�_�������ꍇ1.0f�ɂ��ē������~�߂�
		else {
			timeRate = 1.0f;
		}
	}
	
	time += 0.5f;
	//timeRate / FPS�@��1�b�̃J�E���g��nowTime�ɑ������ 
	timeRate = time / 120;

	timeRate = min(timeRate / maxTime, 1.0f);
	//�x�W�G�Ȑ�
	/*Vector3 a = lerp(start, p1, nowtime);
	Vector3 b = lerp(p1, end, nowtime);
	position = ease_in_out(a, b, nowtime);*/
	position = SplinePosition(points, startIndex, timeRate);
	//�����_�ړ��i�x�N�g���̉��Z�j
	worldTransform.translation_ = position;
}

void RailCamera::RailIni()
{
	
	//�����̐��{1���̒��_���W���v�Z
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float f = 1.0f / segmentCount * i;
		//Vector3 pos = 
		//�`��p���_���X�g�ɒǉ�
		//pointsDrawing.push_back(pos);
	}

}

void RailCamera::DrawRail()
{
	for (int i = 0; i < points.size() - 3; i++) {
		for (int j = 0; j < 99; j++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d(
				positions[i][j] ,
				positions[i][j + 1] ,
				Vector4(1,1,1,1));
		}
	}
	
}
