#include "Matrix.h"
#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"


Matrix::Matrix()
{
	matResult.Trans = MathUtility::Matrix4Identity();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matResult.Scale.m[i][j] = 0;
			matResult.Rot.m[i][j] = 0;
		}
	}
	
}

//�P�ʍs��
void Matrix::UnitMatrix(Matrix4& m) {
	Matrix4 unitMatrix;

	for (int i = 0; i < 4; i++) {
		unitMatrix.m[i][i] = 1.0f;
		for (int j = 0; j < 4; j++) {
			if (i != j) {
				unitMatrix.m[i][j] = 0.0f;
			}
		}
	}

	m = unitMatrix;
}

//�����ݒ�
//�X�P�[�����O�`�F���W
void Matrix::ScaleChange(WorldTransform& worldTransform, float m1, float m2, float m3, float m4) {
	//X,Y,Z�����̃X�P�[�����O��ݒ�
	worldTransform.scale_ = { m1,m2,m3 };
	//�X�P�[�����O�s���錾

	matResult.Scale.m[0][0] = worldTransform.scale_.x;
	matResult.Scale.m[1][1] = worldTransform.scale_.y;
	matResult.Scale.m[2][2] = worldTransform.scale_.z;
	matResult.Scale.m[3][3] = m4;
}
//��]
void Matrix::RotaChange(WorldTransform& worldTransform, float m1, float m2, float m3) {
	//X�AY�AZ������̉�]�p��ݒ�
	worldTransform.rotation_ = { m1,m2,m3 };
	//X����]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//�e���̉�]�s�������
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}
//���s�ړ�
void Matrix::ChangeTranslation(WorldTransform& worldTransform, float tx, float ty, float tz)
{
	//�w�C�x�C�y������̕��s�ړ���ݒ�
	worldTransform.translation_ = { tx,ty,tz };

	matResult.Trans.m[3][0] = worldTransform.translation_.x;
	matResult.Trans.m[3][1] = worldTransform.translation_.y;
	matResult.Trans.m[3][2] = worldTransform.translation_.z;
}

//�A������
void Matrix::ScaleChange(WorldTransform& worldTransform)
{
	matResult.Scale.m[0][0] = worldTransform.scale_.x;
	matResult.Scale.m[1][1] = worldTransform.scale_.y;
	matResult.Scale.m[2][2] = worldTransform.scale_.z;
	matResult.Scale.m[3][3] = 1;
}
void Matrix::RotaChange(WorldTransform& worldTransform)
{
	//X����]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//�e���̉�]�s�������
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}
void Matrix::ChangeTranslation(WorldTransform& worldTransform)
{
	matResult.Trans.m[3][0] = worldTransform.translation_.x;
	matResult.Trans.m[3][1] = worldTransform.translation_.y;
	matResult.Trans.m[3][2] = worldTransform.translation_.z;
}

//�g�ݍ��킹
void Matrix::UpdateMatrix(WorldTransform& worldTransform)
{
	//�s��̍���

	ScaleChange(worldTransform);
	RotaChange(worldTransform);
	ChangeTranslation(worldTransform);


	UnitMatrix(worldTransform.matWorld_);
	worldTransform.matWorld_ *= matResult.Scale;
	worldTransform.matWorld_ *= matResult.Rot;
	worldTransform.matWorld_ *= matResult.Trans;

	if (worldTransform.parent_) {
		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	}

	//�s��̓]��
	worldTransform.TransferMatrix();
}



